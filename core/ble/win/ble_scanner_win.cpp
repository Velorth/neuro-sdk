#include "ble/win/ble_scanner_win.h"

#include <windows.h>
#include <SetupAPI.h>
#include <regstr.h>
#include <bthdef.h>
#include <devguid.h>
#include <Bluetoothleapis.h>
#include <sstream>
#include "ble/win/ble_device_win.h"
#include "ble/win/gatt_wrapper.h"
#include "logger.h"

namespace Neuro {

std::string to_narrow(const wchar_t *s, char def = '?', const std::locale& loc = std::locale()){
    std::ostringstream stm;
    while (*s != L'\0') {
        stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, def);
    }
    return stm.str();
}

std::string getErrorString(DWORD error_message_id){
    if(error_message_id == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error_message_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);

    return message;
}

GUID guidFromString(const std::string &guid_string){
    GUID guid;
    sscanf(guid_string.c_str(),
           "%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
           &guid.Data1, &guid.Data2, &guid.Data3,
           &guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
            &guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7] );
    return guid;
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

std::string extractBluetoothAddressFromDeviceInstanceId(const std::string& instance_id) {
  size_t start = instance_id.find("_");
  if (start == std::string::npos) {
    return "";
  }

  size_t end = instance_id.find("\\", start);
  if (end == std::string::npos) {
    return "";
  }
  start++;
  std::string address = instance_id.substr(start, end - start);
  return address;
}

DevInterfaceDetailData getInterfaceDetails(const DeviceInfoListPtr &device_info_list,
                                           SP_DEVICE_INTERFACE_DATA iface_data,
                                           SP_DEVINFO_DATA &info_data){
    DWORD size = 0;
    if (!SetupDiGetDeviceInterfaceDetail(device_info_list.get(), &iface_data, NULL, 0, &size, 0)){
        if (GetLastError() == ERROR_NO_MORE_ITEMS){
            LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] No more items", "BleScannerWrapper", __FUNCTION__);
            return DevInterfaceDetailData(nullptr, &UniversalGlobalDeleter);
        }

        auto pInterfaceDetailData = make_dev_interface_detail_data(size);
        pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        if (!SetupDiGetDeviceInterfaceDetail(device_info_list.get(), &iface_data, pInterfaceDetailData.get(), size, &size, &info_data)){
            LoggerFactory::getCurrentPlatformLogger()->warn("[%s: %s] Failed get device interface details", "BleScannerWrapper", __FUNCTION__);
            return DevInterfaceDetailData(nullptr, &UniversalGlobalDeleter);
        }
        return pInterfaceDetailData;
    }
    return DevInterfaceDetailData(nullptr, &UniversalGlobalDeleter);
}

std::string readStringRegistryProperty(const DeviceInfoListPtr &device_info_list,
                                       SP_DEVINFO_DATA dev_info_data,
                                       DWORD property){
    LPTSTR propertyBuffer = NULL;
    DWORD propertyBufferSize = 0;
    while (!SetupDiGetDeviceRegistryProperty(
               device_info_list.get(),
               &dev_info_data,
               property,
               NULL,
               (PBYTE)propertyBuffer,
               propertyBufferSize,
               &propertyBufferSize)){
        auto lastError = GetLastError();
        if (lastError == ERROR_INSUFFICIENT_BUFFER){
            if (propertyBuffer) LocalFree(propertyBuffer);
            propertyBuffer = (wchar_t *)LocalAlloc(LPTR,propertyBufferSize * 2);
        }else{
            auto excMsg = std::string("Unable read property: Error code ") + std::to_string(lastError) + std::string(" - ") + getErrorString(lastError);
            throw std::runtime_error(excMsg.c_str());
        }
    }
    return to_narrow(propertyBuffer);
}

std::string getDeviceName(const DeviceInfoListPtr &device_info_list, SP_DEVINFO_DATA dev_info_data){
    try{
        return readStringRegistryProperty(device_info_list, dev_info_data, SPDRP_FRIENDLYNAME);
    }
    catch(std::runtime_error &e){
        LoggerFactory::getCurrentPlatformLogger()->error("[%s: %s] %s", "BleScannerWrapper", __FUNCTION__, e.what());
        throw std::runtime_error("Unable retreive device name");
    }
}

std::string getDeviceAddress(const DeviceInfoListPtr &device_info_list, SP_DEVINFO_DATA dev_info_data){
    try{
        auto instanceId = readStringRegistryProperty(device_info_list, dev_info_data, SPDRP_HARDWAREID);
        return extractBluetoothAddressFromDeviceInstanceId(instanceId);
    }
    catch(std::runtime_error &e){
        LoggerFactory::getCurrentPlatformLogger()->error("[%s: %s] %s", "BleScannerWrapper", __FUNCTION__, e.what());
        throw std::runtime_error("Unable retreive device address");
    }
}

std::vector<std::unique_ptr<BleDevice>> findDevicesByGuid(GUID serviceGuid){
    LoggerFactory::getCurrentPlatformLogger()->trace("[%s: %s] Looking for devices with service GUID %s", "BleScannerWrapper", __FUNCTION__, to_string(serviceGuid).c_str());
    std::vector<std::unique_ptr<BleDevice>> foundDevices;
    auto deviceListPtr = make_device_list(serviceGuid);
    if (deviceListPtr.get() == INVALID_HANDLE_VALUE){
        LoggerFactory::getCurrentPlatformLogger()->warn("[%s: %s] Invalid handle", "BleScannerWrapper", __FUNCTION__);
        return foundDevices;
    }

    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(deviceListPtr.get(), NULL, &serviceGuid, i, &deviceInterfaceData); i++){
        auto deviceInterfaceDetails = getInterfaceDetails(deviceListPtr, deviceInterfaceData, deviceInfoData);
        if (deviceInterfaceDetails == nullptr){
            break;
        }
        try {
            auto deviceName = getDeviceName(deviceListPtr, deviceInfoData);
            auto deviceAddress = getDeviceAddress(deviceListPtr, deviceInfoData);
            auto deviceHandle = make_handle(deviceInterfaceDetails);
            if (deviceHandle != nullptr){
                foundDevices.push_back(std::make_unique<BleDeviceWin>(std::move(deviceHandle), deviceName, deviceAddress));
            }else{
                LoggerFactory::getCurrentPlatformLogger()->warn("[%s: %s] Unable to create handle for device %s [%s]: Handle is null",
                                                                "BleScannerWrapper",
                                                                __FUNCTION__,
                                                                deviceName.c_str(),
                                                                deviceAddress.c_str());
            }
        }
        catch(std::runtime_error &e){
            LoggerFactory::getCurrentPlatformLogger()->warn("[%s: %s] Unable to create handle for device: %s",
                                                            "BleScannerWrapper",
                                                            __FUNCTION__,
                                                            e.what());
        }
    }
    return foundDevices;
}

std::vector<std::unique_ptr<BleDevice>> findDevicesWithFilters(const std::vector<std::shared_ptr<DeviceGattInfo>> &gattFilters){
    auto serviceGuids = getServiceGuids(gattFilters);
    std::vector<std::unique_ptr<BleDevice>> devices;
    for (auto guid : serviceGuids){
        auto guidDevices = findDevicesByGuid(guid);
        devices.insert(devices.end(),
                       std::make_move_iterator(guidDevices.begin()),
                       std::make_move_iterator(guidDevices.end()));
    }
    return devices;
}

BleScannerWin::BleScannerWin():
    mSpawnScanTaskLoop(&BleScannerWin::spawnScanTask, std::chrono::milliseconds(300), this){
}

void BleScannerWin::startScan(){
    mFoundDeviceAddresses.clear();
    emulator.startScan();
    mIsScanning.store(true);
}

void BleScannerWin::stopScan(){
    emulator.stopScan();    
    mIsScanning.store(false);
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
        if (mIsScanning.load()){
            auto devices = findDevicesWithFilters(mFilterCollection);
            for (auto &device : devices){
                onDeviceFound(std::move(device));
            }
        }
    });
}

void BleScannerWin::onDeviceFound(std::unique_ptr<BleDevice> device_ptr){
    auto deviceAddress = device_ptr->getNetAddress();
    if (mFoundDeviceAddresses.find(deviceAddress) != mFoundDeviceAddresses.end()){
        LOG_TRACE_V("Device %s already found", deviceAddress.c_str());
        return;
    }
    LOG_DEBUG_V("Found device %s [%s]", device_ptr->getName().c_str(), deviceAddress.c_str());
    mFoundDeviceAddresses.insert(deviceAddress);
    if (mDeviceFoundCallback) mDeviceFoundCallback(std::move(device_ptr));
}

}
