#include "ble/win/ble_scanner_win.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include "ble/win/ble_device_win.h"
#include "ble/win/gatt_wrapper.h"
#include "logger.h"

namespace Neuro {

std::string extractBluetoothAddressFromDeviceInstanceId(const std::string& instance_id) {
  size_t start = instance_id.find("_");
  if (start == std::string::npos) {
    return "";
  }

  size_t end = instance_id.find("\\", start);

  start++;
  std::string address = instance_id.substr(start, end - start);
  std::transform(address.begin(), address.end(), address.begin(), [](unsigned char ch){return std::toupper(ch);});
  address.insert(address.begin() + 2, ':');
  address.insert(address.begin() + 5, ':');
  address.insert(address.begin() + 8, ':');
  address.insert(address.begin() + 11, ':');
  address.insert(address.begin() + 14, ':');
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

std::string getDeviceName(const DeviceInfoListPtr &device_info_list, SP_DEVINFO_DATA dev_info_data){
    try{
        return read_registry_property<SPDRP_FRIENDLYNAME>(device_info_list, dev_info_data);
    }
    catch(std::runtime_error &e){
        LoggerFactory::getCurrentPlatformLogger()->error("[%s: %s] %s", "BleScannerWrapper", __FUNCTION__, e.what());
        throw std::runtime_error("Unable retreive device name");
    }
}

std::string getDeviceAddress(const DeviceInfoListPtr &device_info_list, SP_DEVINFO_DATA dev_info_data){
    try{
        auto instanceId = read_registry_property<SPDRP_HARDWAREID>(device_info_list, dev_info_data);
        return extractBluetoothAddressFromDeviceInstanceId(instanceId);
    }
    catch(std::runtime_error &e){
        LoggerFactory::getCurrentPlatformLogger()->error("[%s: %s] %s", "BleScannerWrapper", __FUNCTION__, e.what());
        throw std::runtime_error("Unable retreive device address");
    }
}



BleScannerWin::BleScannerWin():
    mSpawnScanTaskLoop(&BleScannerWin::spawnScanTask, std::chrono::milliseconds(1000), this){
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

void BleScannerWin::setFilter(std::vector<std::string> nameFilter){
    emulator.setFilter(nameFilter);
    mFilterCollection = nameFilter;
}

void BleScannerWin::subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)> callback){
    emulator.subscribeDeviceFound(callback);
    mDeviceFoundCallback = callback;
}

bool BleScannerWin::isScanning(){
    return emulator.isScanning() || mIsScanning.load();
}

void BleScannerWin::releaseDevice(std::string name, std::string address){
    emulator.releaseDevice(name, address);
    LOG_DEBUG_V("Release device %s [%s]", name.c_str(), address.c_str());
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

std::vector<std::unique_ptr<BleDevice>> BleScannerWin::findDevicesWithFilters(const std::vector<std::string> &nameFilters){
    std::vector<std::unique_ptr<BleDevice>> devices;
    auto btDevicesListPtr = make_device_list(GUID_BLUETOOTHLE_DEVICE_INTERFACE);
    for (auto &name : nameFilters) {
        auto bleInfo = BleDeviceInfo::fromDeviceName(name);
        auto uuid = bleInfo->getGattInfo()->deviceServiceUUID();
        GUID serviceGUID = guid_from_string(uuid);

        auto serviceListPtr = make_device_list(serviceGUID);
        if (serviceListPtr.get() == INVALID_HANDLE_VALUE){
            LOG_WARN("Invalid handle");
            return devices;
        }

        SP_DEVICE_INTERFACE_DATA serviceInterfaceData;
        serviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

        SP_DEVINFO_DATA serviceInfoData;
        serviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        for (DWORD i = 0; SetupDiEnumDeviceInterfaces(serviceListPtr.get(), NULL, &serviceGUID, i, &serviceInterfaceData); i++){
            auto serviceInterfaceDetails = getInterfaceDetails(serviceListPtr, serviceInterfaceData, serviceInfoData);
            if (serviceInterfaceDetails == nullptr){
                continue;
            }
            try {
                SP_DEVINFO_DATA parentDeviceInfoData;
                parentDeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
                SetupDiEnumDeviceInfo(btDevicesListPtr.get(), i, &parentDeviceInfoData);
                auto r = CM_Get_Parent(&parentDeviceInfoData.DevInst, serviceInfoData.DevInst, 0);
                if (r != CR_SUCCESS){
                    LOG_WARN_V("Unable to find parent device for service %s", uuid.c_str());
                    continue;
                }
                auto deviceAddress = getDeviceAddress(btDevicesListPtr, parentDeviceInfoData);
                auto deviceName = getDeviceName(btDevicesListPtr, parentDeviceInfoData);
				/*if (deviceName != bleInfo->getName()) {
					LOG_TRACE_V("Wrong device name: %s", deviceName.c_str());
					continue;
				}*/
                if (mFoundDeviceAddresses.find(deviceAddress) != mFoundDeviceAddresses.end()){
                    LOG_TRACE_V("Device %s already found", deviceAddress.c_str());
                    continue;
                }
                LOG_TRACE_V("Creating service handle for device %s [%s]", deviceName.c_str(), deviceAddress.c_str());
                auto serviceHandle = make_handle(serviceInterfaceDetails);
                if (serviceHandle != nullptr){
                    devices.push_back(std::make_unique<BleDeviceWin>(std::move(serviceHandle), deviceName, deviceAddress));
                    mFoundDeviceAddresses.insert(deviceAddress);
                }else{
                    LOG_WARN_V("Unable to create handle for device %s [%s]: Handle is null", deviceName.c_str(), deviceAddress.c_str());
                }
            }
            catch(std::runtime_error &e){
                LOG_WARN_V("Unable to create handle for device: %s", e.what());
            }
        }
    }
    return devices;
}

void BleScannerWin::onDeviceFound(std::unique_ptr<BleDevice> &&device_ptr){
    LOG_DEBUG_V("Found device %s [%s]", device_ptr->getName().c_str(), device_ptr->getNetAddress().c_str());
    if (mDeviceFoundCallback)
        mDeviceFoundCallback(std::move(device_ptr));
}

}
