#include "ble/win/ble_device_win.h"

namespace Neuro {

BleDeviceWin::BleDeviceWin(DeviceHandle &&device_handle, std::string name, std::string address) :
    mDeviceHandle(std::move(device_handle)),
    mName(name),
    mAddress(address){
}

void BleDeviceWin::connect(){

}

void BleDeviceWin::disconnect(){

}

void BleDeviceWin::close(){

}

bool BleDeviceWin::sendCommand(const std::vector<Byte> &commandData){
    return false;
}

BleDeviceState BleDeviceWin::getState() const {
    return BleDeviceState::Error;
}

std::string BleDeviceWin::getName() const {
    return mName;
}

std::string BleDeviceWin::getNetAddress() const {
    return mAddress;
}

}
