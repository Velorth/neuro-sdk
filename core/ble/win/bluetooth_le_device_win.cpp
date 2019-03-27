#include "ble/win/bluetooth_le_device_win.h"
#include "device/device_info.h"

using WindowsBleDevice = winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
using winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;

namespace Neuro {

WindowsBleDevice device_from_address(const DeviceAddressType &address) {
	auto deviceAsyncOperation = WindowsBleDevice::FromBluetoothAddressAsync(static_cast<uint64_t>(address));
	if (deviceAsyncOperation == nullptr) {
		throw std::runtime_error("Device is not reachable");
	}
	auto device = deviceAsyncOperation.get();
	if (device == nullptr) {
		throw std::runtime_error("Device is not reachable");
	}
	return device;
}

BluetoothLEDeviceWin::BluetoothLEDeviceWin(const DeviceInfo &device_info):
	mDevice(device_from_address(device_info.Address)){
	mDevice.ConnectionStatusChanged([=](const WindowsBleDevice &, auto) {
		if (mDevice.ConnectionStatus() == BluetoothConnectionStatus::Disconnected) {
			mStateNotifier.notifyAll(BleDeviceState::Disconnected);
		}
		else {
			mStateNotifier.notifyAll(BleDeviceState::Connected);
		}
	});
}

BluetoothLEDeviceWin::~BluetoothLEDeviceWin() {
	try {
		mDevice.Close();
	}
	catch(...){}
}

std::vector<GattServiceWin> BluetoothLEDeviceWin::services() const {
	auto result = mDevice.GetGattServicesAsync().get().Services();
	std::vector<GattServiceWin> services;
	std::transform(begin(result), end(result), std::back_inserter(services), [](const auto &service) { return GattServiceWin(service); });
	return  services;
}

BleDeviceState BluetoothLEDeviceWin::connectionState() const {
	if (mDevice.ConnectionStatus() == BluetoothConnectionStatus::Connected) {
		return BleDeviceState::Connected;
	}
	else {
		return BleDeviceState::Disconnected;
	}
}

ListenerPtr<void, BleDeviceState> BluetoothLEDeviceWin::subscribeStateChanged(std::function<void(BleDeviceState)> callback) {
	return mStateNotifier.addListener(callback);
}

BluetoothLEDeviceWin make_bluetooth_le_device(const DeviceInfo& device_info) {
	winrt::init_apartment();
	return BluetoothLEDeviceWin(device_info);
}

}
