#include "ble/win/ble_scanner_win.h"

#include <windows.h>
#include <atlbase.h>
#include <SetupAPI.h>
#include <regstr.h>
#include <bthdef.h>
#include <devguid.h>
#include <Bluetoothleapis.h>
#include "ble/win/ble_device_win.h"
#include "logger.h"

namespace Neuro {

GUID guidFromString(const std::string &guid_string){
    USES_CONVERSION;
    GUID result;
    CLSIDFromString(A2OLE(guid_string.c_str()), &result);
    return result;
}

std::vector<GUID> getServiceGuids(const std::vector<std::shared_ptr<DeviceGattInfo>> &gattInfoCollection){
    std::vector<GUID> serviceUUIDs;
    std::transform(gattInfoCollection.begin(),
                   gattInfoCollection.end(),
                   std::back_inserter(serviceUUIDs),
                   [](const auto &gattInfoPtr){
        return guidFromString(gattInfoPtr->deviceServiceUUID());
    });
    return serviceUUIDs;
}

std::unique_ptr<BleDevice> createDeviceFromHandle(HANDLE device_handle){
    return std::unique_ptr<BleDevice>(new BleDeviceWin);
}

std::vector<std::unique_ptr<BleDevice>> findDevicesByGuid(GUID serviceGuid){

    GUID BluetoothInterfaceGUID = serviceGuid;
    std::vector<std::unique_ptr<BleDevice>> hCommResult;
    auto hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

    if (hDI == INVALID_HANDLE_VALUE)
        return hCommResult;

    SP_DEVICE_INTERFACE_DATA did;
    did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    SP_DEVINFO_DATA dd;
    dd.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, i, &did); i++)
    {
        SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

        DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        DWORD size = 0;

        if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0))
        {
            int err = GetLastError();

            if (err == ERROR_NO_MORE_ITEMS) break;

            PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

            pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
                break;

            auto hComm = CreateFile(
                        pInterfaceDetailData->DevicePath,
                        GENERIC_WRITE | GENERIC_READ,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        0,
                        NULL);
            if (hComm!=nullptr){
                hCommResult.push_back(createDeviceFromHandle(hComm));
            }

            GlobalFree(pInterfaceDetailData);
        }
    }
    SetupDiDestroyDeviceInfoList(hDI);

    return hCommResult;
}

std::vector<std::unique_ptr<BleDevice>> findDevicesWithFilters(const std::vector<std::shared_ptr<DeviceGattInfo>> &gattFilters){
    auto serviceGuids = getServiceGuids(gattFilters);
    std::vector<std::unique_ptr<BleDevice>> devices;
    for (auto guid : serviceGuids){
        auto guidDevices = findDevicesByGuid(guid);
        devices.insert(devices.end(), guidDevices.begin(), guidDevices.end());
    }
    return devices;
}

BleScannerWin::BleScannerWin():
    mSpawnScanTaskLoop(&BleScannerWin::spawnScanTask, std::chrono::microseconds(300), this){

}

void BleScannerWin::startScan(){
    emulator.startScan();
}

void BleScannerWin::stopScan(){
    emulator.stopScan();
}

std::unique_ptr<BleDevice> BleScannerWin::getDeviceByAddress(std::string address){
    return emulator.getDeviceByAddress(address);
}

void BleScannerWin::setFilter(std::vector<std::shared_ptr<DeviceGattInfo>> gattFilter){
    std::vector<std::string> filterNames;
    for (auto &gattInfo: gattFilter) {
        auto names = gattInfo->getValidBtNames();
        filterNames.insert(filterNames.end(), names.begin(), names.end());
    }
    emulator.setFilter(filterNames);
    mFilterCollection = gattFilter;
}

void BleScannerWin::subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)> callback){
    emulator.subscribeDeviceFound(callback);
}

bool BleScannerWin::isScanning(){
    return emulator.isScanning() || mIsScanning.load();
}

void BleScannerWin::releaseDevice(std::string name, std::string address){
    emulator.releaseDevice(name, address);
    auto deviceIt = mFoundDeviceAddresses.find(address);
    if (deviceIt != mFoundDeviceAddresses.end()){
        mFoundDeviceAddresses.erase(deviceIt);
    }
}

void BleScannerWin::spawnScanTask(){
    mScanTaskQueue.exec([=](){
        auto devices = findDevicesWithFilters(mFilterCollection);
        for (auto &device : devices){
            onDeviceFound(std::move(device));
        }
    });
}

void BleScannerWin::onDeviceFound(std::unique_ptr<BleDevice> device_ptr){
    auto deviceAddress = device_ptr->getNetAddress();
    if (mFoundDeviceAddresses.find(deviceAddress) != mFoundDeviceAddresses.end()){
        LOG_TRACE_V("Device %s already found", deviceAddress);
        return;
    }
    LOG_DEBUG_V("Found device %s [%s]", device_ptr->getName(), deviceAddress);
    mFoundDeviceAddresses.insert(deviceAddress);
    if (mDeviceFoundCallback) mDeviceFoundCallback(std::move(device_ptr));
}

}
