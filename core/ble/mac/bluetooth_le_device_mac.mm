#include "ble/mac/bluetooth_le_device_mac.h"
#include "device/device_info.h"

namespace Neuro {

BluetoothLEDeviceMac::BluetoothLEDeviceMac(const DeviceInfo &device_info):
	mDevice(){
	mDevice.ConnectionStatusChanged([=](const WindowsBleDevice &, auto) {
        mStateNotifier.notifyAll(connectionState());
	});
}

BluetoothLEDeviceMac::~BluetoothLEDeviceMac() {
	try {
		mDevice.Close();
	}
	catch(...){}
}

std::vector<GattServiceMac> BluetoothLEDeviceMac::services() const {
	auto result = mDevice.GetGattServicesAsync().get().Services();
	std::vector<GattServiceMac> services;
	std::transform(begin(result), end(result), std::back_inserter(services), [](const auto &service) { return GattServiceMac(service); });
	return  services;
}

BleDeviceState BluetoothLEDeviceMac::connectionState() const {
	
}

ListenerPtr<void, BleDeviceState> BluetoothLEDeviceMac::subscribeStateChanged(std::function<void(BleDeviceState)> callback) {
	return mStateNotifier.addListener(callback);
}

BluetoothLEDeviceMac make_bluetooth_le_device(const DeviceInfo& device_info) {
	return BluetoothLEDeviceMac(device_info);
}

}
