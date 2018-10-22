#include "ble/win/ble_device_win.h"
#include "string_utils.h"

using winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
using winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;
using winrt::Windows::Foundation::AsyncStatus;
using winrt::Windows::Foundation::IAsyncOperation;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs;
using winrt::Windows::Storage::Streams::DataReader;

namespace Neuro {

GUID guid_from_string(const std::string &guid_string) {
	GUID guid;
	sscanf(guid_string.c_str(),
		"%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
		&guid.Data1, &guid.Data2, &guid.Data3,
		&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
		&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);
	return guid;
}


BleDeviceWin::BleDeviceWin(unsigned long long address, const std::string &name) :
	BleDevice(BleDeviceInfo::fromDeviceName(name)),
	mName(name),
	mAddress(address) {
	
}

void BleDeviceWin::connect() {
	if (mBluetoothDevice != nullptr) {
		return;
	}

	auto asyncOperation = BluetoothLEDevice::FromBluetoothAddressAsync(mAddress);
	asyncOperation.Completed([=](IAsyncOperation<BluetoothLEDevice> const& sender, AsyncStatus) {
		mBluetoothDevice = sender.GetResults();
		mBluetoothDevice.ConnectionStatusChanged([=](const BluetoothLEDevice &, auto) {
			mIsConnected = mBluetoothDevice.ConnectionStatus() == BluetoothConnectionStatus::Connected;
		});
		if (mBluetoothDevice.ConnectionStatus() == BluetoothConnectionStatus::Connected) {
			mService = mBluetoothDevice.GetGattService(guid_from_string(mDeviceInfo->getGattInfo()->deviceServiceUUID()));
			if (mService == nullptr)
				return;
			auto rxChars = mService.GetCharacteristics(guid_from_string(mDeviceInfo->getGattInfo()->rxCharacteristicUUID()));
			if (rxChars.Size() != 1)
				return;
			mRxCharacteristic = rxChars.GetAt(0);

			auto txChars = mService.GetCharacteristics(guid_from_string(mDeviceInfo->getGattInfo()->txCharacteristicUUID()));
			if (txChars.Size() != 1)
				return;
			mTxCharacteristic = txChars.GetAt(0);

			const auto statusCharUUIDString = mDeviceInfo->getGattInfo()->statusCharacteristicUUID();
			if (!statusCharUUIDString.empty()) {
				mHasStatusCharacteristic = true;
				auto statusChars = mService.GetCharacteristicsForUuidAsync(guid_from_string(statusCharUUIDString));
				if (statusChars.Size() != 1)
					return;
				mStatusCharacteristic = statusChars.GetAt(0);
				/*auto asyncResult = mStatusCharacteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify);
				asyncResult.Completed([=](auto...) {
					mStatusCharacteristic.ValueChanged([=](const GattCharacteristic &, const GattValueChangedEventArgs &args) {
						auto dataReader = DataReader::FromBuffer(args.CharacteristicValue());
						std::vector<Byte> statusData(args.CharacteristicValue().Length());
						dataReader.ReadBytes(statusData);
					});
				});*/
			}
			else {
				mHasStatusCharacteristic = false;
				onConnected();
			}
		}
	});
}

void BleDeviceWin::disconnect(){
    if (mBluetoothDevice == nullptr){
        return;
    }

	mBluetoothDevice = nullptr;
	mService = nullptr;
	mTxCharacteristic = nullptr;
	mRxCharacteristic = nullptr;
	mStatusCharacteristic = nullptr;
	onDisconnected();
}

void BleDeviceWin::close(){
    disconnect();
}

bool BleDeviceWin::sendCommand(const std::vector<Byte> &commandData){
    if (!mIsConnected){
        return false;
    }
	return false;
}

BleDeviceState BleDeviceWin::getState() const {
    return mIsConnected ? BleDeviceState::Connected : BleDeviceState::Disconnected;
}

std::string BleDeviceWin::getName() const {
    return mName;
}

std::string BleDeviceWin::getNetAddress() const {
    return to_hex_string(mAddress);
}

void BleDeviceWin::onConnected() {
	mIsConnected = true;
	if (deviceStateChangedCallback)
		deviceStateChangedCallback(BleDeviceState::Connected, BleDeviceError::NoError);
}

void BleDeviceWin::onDisconnected() {
	mIsConnected = false;
	if (deviceStateChangedCallback)
		deviceStateChangedCallback(BleDeviceState::Disconnected, BleDeviceError::NoError);
}

}
