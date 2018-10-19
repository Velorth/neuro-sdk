#include "ble/win/ble_device_win.h"
#include "string_utils.h"

using winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;

namespace Neuro {

BleDeviceWin::BleDeviceWin(unsigned long long address, const std::string &name) :
	BleDevice(BleDeviceInfo::fromDeviceName(name)),
	mName(name),
	mAddress(address) {
	
}

void BleDeviceWin::connect(){
    if(mIsConnected.load()){
        return;
    }
    performConnect();
}

void BleDeviceWin::disconnect(){
    if (!mIsConnected.load()){
        return;
    }
    performDisconnect();
}

void BleDeviceWin::close(){
    disconnect();
}

bool BleDeviceWin::sendCommand(const std::vector<Byte> &commandData){
    if (!mIsConnected.load()){
        return false;
    }
	return false;
}

BleDeviceState BleDeviceWin::getState() const {
    return mIsConnected.load() ? BleDeviceState::Connected : BleDeviceState::Disconnected;
}

std::string BleDeviceWin::getName() const {
    return mName;
}

std::string BleDeviceWin::getNetAddress() const {
    return to_hex_string(mAddress);
}

void BleDeviceWin::performConnect(){
		if (mBluetoothDevice == nullptr) {
			auto asyncOperation = BluetoothLEDevice::FromBluetoothAddressAsync(mAddress);
			asyncOperation.Completed([=](auto sender, auto args) {
				mBluetoothDevice = args;
			});
		}
		else {
			onConnected();
		}
}

void BleDeviceWin::onConnected() {
	mIsConnected.store(true);
	if (deviceStateChangedCallback)
		deviceStateChangedCallback(BleDeviceState::Connected, BleDeviceError::NoError);
}

void BleDeviceWin::performDisconnect(){
	onDisconnected();
}

void BleDeviceWin::onDisconnected() {
	mIsConnected.store(false);
	if (deviceStateChangedCallback)
		deviceStateChangedCallback(BleDeviceState::Disconnected, BleDeviceError::NoError);
}

}
