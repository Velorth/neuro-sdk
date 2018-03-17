#include "ble/win/ble_scanner_win.h"

namespace Neuro {

void BleScannerWin::startScan(){
    emulator.startScan();
}

void BleScannerWin::stopScan(){
    emulator.stopScan();
}

std::unique_ptr<BleDevice> BleScannerWin::getDeviceByAddress(std::string address){
    return emulator.getDeviceByAddress(address);
}

void BleScannerWin::setFilter(std::vector<std::string> filter){
    emulator.setFilter(filter);
}

void BleScannerWin::subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)> callback){
    emulator.subscribeDeviceFound(callback);
}

bool BleScannerWin::isScanning(){
    return emulator.isScanning();
}

void BleScannerWin::releaseDevice(std::string name, std::string address){
    emulator.releaseDevice(name, address);
}

}
