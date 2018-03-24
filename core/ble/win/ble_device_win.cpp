#include "ble/win/ble_device_win.h"

namespace Neuro {

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
    return BleDeviceState::ERROR;
}

std::string BleDeviceWin::getName() const {
    return "null";
}

std::string BleDeviceWin::getNetAddress() const {
    return "null";
}

}
