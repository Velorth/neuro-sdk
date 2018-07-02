#include "ble/win/ble_device_win.h"
#include "logger.h"

namespace Neuro {

BleDeviceWin::BleDeviceWin(DeviceHandle &&device_handle, std::string name, std::string address) :
    BleDevice(BleDeviceInfo::fromDeviceName(name)),
    mDeviceHandle(std::move(device_handle)),
    mName(name),
    mAddress(address){
    mService = get_service(mDeviceHandle, mDeviceInfo->getGattInfo()->deviceServiceUUID());
    mRxCharacteristic = get_device_characteristic(mDeviceHandle, mService, mDeviceInfo->getGattInfo()->rxCharacteristicUUID());
    mTxCharacteristic = get_device_characteristic(mDeviceHandle, mService, mDeviceInfo->getGattInfo()->txCharacteristicUUID());
}

void BleDeviceWin::connect(){

}

void BleDeviceWin::disconnect(){

}

void BleDeviceWin::close(){

}

bool BleDeviceWin::sendCommand(const std::vector<Byte> &commandData){
    if (!mIsConnected.load()){
        return false;
    }
    return write_characteristic(mDeviceHandle, mTxCharacteristic, commandData);
}

BleDeviceState BleDeviceWin::getState() const {
    return mIsConnected.load() ? BleDeviceState::Connected : BleDeviceState::Disconnected;
}

std::string BleDeviceWin::getName() const {
    return mName;
}

std::string BleDeviceWin::getNetAddress() const {
    return mAddress;
}

void BleDeviceWin::spawnGetServiceTask(){
    mGetServiceTaskQueue.exec([=](){
        //auto services = getDeviceServices(mDeviceHandle);

    });
}

}
