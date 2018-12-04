#include "ble/win/ble_device_win.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#include "string_utils.h"

using winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
using winrt::Windows::Devices::Bluetooth::BluetoothConnectionStatus;
using winrt::Windows::Foundation::AsyncStatus;
using winrt::Windows::Foundation::IAsyncOperation;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs;
using winrt::Windows::Storage::Streams::DataReader;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicsResult;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult;

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
		findServiceAndChars();
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

void BleDeviceWin::findServiceAndChars() {
	if (mBluetoothDevice.ConnectionStatus() == BluetoothConnectionStatus::Connected) {
		auto serviceResult = mBluetoothDevice.GetGattServicesForUuidAsync(guid_from_string(mDeviceInfo->getGattInfo()->deviceServiceUUID()));
		serviceResult.Completed([=](IAsyncOperation<GattDeviceServicesResult> sender, AsyncStatus) {
			auto result = sender.GetResults();
			auto services = result.Services();
			if (services.Size() != 1) {
				return;
			}
			mService = services.GetAt(0);			

			findAllCharacteristics();
		});		
	}
}

void BleDeviceWin::findAllCharacteristics() {
	auto rxCharsResult = mService.GetCharacteristicsForUuidAsync(guid_from_string(mDeviceInfo->getGattInfo()->rxCharacteristicUUID()));
	rxCharsResult.Completed([=](IAsyncOperation<GattCharacteristicsResult> sender, AsyncStatus) {
		auto result = sender.GetResults();
		auto rxChars = result.Characteristics();
		if (rxChars.Size() != 1)
			return;
		mRxCharacteristic = rxChars.GetAt(0);

		auto writeDescriptorResult = mRxCharacteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify);
		writeDescriptorResult.Completed([=](auto...) {
			setRxNotificationsAndFindTx();
		});
	});
}

void BleDeviceWin::setRxNotificationsAndFindTx() {
	mRxCharacteristic.ValueChanged([=](const GattCharacteristic &, const GattValueChangedEventArgs &args) {
		auto dataReader = DataReader::FromBuffer(args.CharacteristicValue());
		std::vector<Byte> data(args.CharacteristicValue().Length());
		dataReader.ReadBytes(data);
		if (dataReceivedCallback) dataReceivedCallback(data);
	});
	findTxAndStausChars();
}

void BleDeviceWin::findTxAndStausChars() {
	auto txCharsResult = mService.GetCharacteristicsForUuidAsync(guid_from_string(mDeviceInfo->getGattInfo()->txCharacteristicUUID()));
	txCharsResult.Completed([=](IAsyncOperation<GattCharacteristicsResult> sender, AsyncStatus) {
		auto result = sender.GetResults();
		auto txChars = result.Characteristics();
		if (txChars.Size() != 1)
			return;
		mTxCharacteristic = txChars.GetAt(0);

		findStatusCharacteristic();
	});
}

void BleDeviceWin::findStatusCharacteristic() {
	const auto statusCharUUIDString = mDeviceInfo->getGattInfo()->statusCharacteristicUUID();
	if (!statusCharUUIDString.empty()) {
		mHasStatusCharacteristic = true;
		auto getCharsResult = mService.GetCharacteristicsForUuidAsync(guid_from_string(statusCharUUIDString));
		getCharsResult.Completed([=](IAsyncOperation<GattCharacteristicsResult> sender, AsyncStatus) {
			auto result = sender.GetResults();
			auto statusChars = result.Characteristics();
			if (statusChars.Size() != 1)
				return;
			mStatusCharacteristic = statusChars.GetAt(0);
			auto writeDescriptorResult = mStatusCharacteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify);
			writeDescriptorResult.Completed([=](auto...) {
				setStatusValueChangedNotifications();
			});
		});
	}
	else {
		mHasStatusCharacteristic = false;
		onConnected();
	}
}

void BleDeviceWin::setStatusValueChangedNotifications() {
	mStatusCharacteristic.ValueChanged([=](const GattCharacteristic &, const GattValueChangedEventArgs &args) {
		auto dataReader = DataReader::FromBuffer(args.CharacteristicValue());
		std::vector<Byte> statusData(args.CharacteristicValue().Length());
		dataReader.ReadBytes(statusData);
		if (statusReceivedCallback) statusReceivedCallback(statusData);
	});
	onConnected();
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
