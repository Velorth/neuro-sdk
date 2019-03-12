#include "ble/win/gatt_characteristic_win.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"
#include "event_notifier.h"

using WinRTGattChar = winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattClientCharacteristicConfigurationDescriptorValue;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattValueChangedEventArgs;
using winrt::Windows::Storage::Streams::DataReader;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristicsResult;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceServicesResult;
using winrt::Windows::Storage::Streams::DataWriter;
using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCommunicationStatus;

namespace Neuro {

struct GattCharacteristicWin::Impl final {
	Impl(WinRTGattChar characteristic):
		mCharacteristic(characteristic){}

	winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic mCharacteristic;
	Notifier<void, const std::vector<unsigned char> &> mDataReceived;
};

GattCharacteristicWin::GattCharacteristicWin(WinRTGattChar characteristic):
	mImpl(std::make_unique<Impl>(characteristic)) {}

GattCharacteristicWin::GattCharacteristicWin(GattCharacteristicWin&&) noexcept = default;

GattCharacteristicWin& GattCharacteristicWin::operator=(GattCharacteristicWin&&) noexcept = default;

GattCharacteristicWin::~GattCharacteristicWin() = default;

GUID GattCharacteristicWin::uuid() const {
	return mImpl->mCharacteristic.Uuid();
}

std::vector<unsigned char> GattCharacteristicWin::read() const {
	auto result = mImpl->mCharacteristic.ReadValueAsync(winrt::Windows::Devices::Bluetooth::BluetoothCacheMode::Uncached).get();
	auto dataReader = DataReader::FromBuffer(result.Value());
	std::vector<unsigned char> data(result.Value().Length());
	dataReader.ReadBytes(data);
	return data;
}

void GattCharacteristicWin::write(const std::vector<unsigned char> &data) {
	DataWriter commandWriter;
	commandWriter.WriteBytes(data);
	const auto writeResult = mImpl->mCharacteristic.WriteValueAsync(commandWriter.DetachBuffer()).get();
	if (writeResult != GattCommunicationStatus::Success) {
		throw std::runtime_error("Failed to write characteristic value");
	}
}

ListenerPtr<void, const std::vector<unsigned char>&>
GattCharacteristicWin::subscribeCharacteristicChanged(const std::function<void(const std::vector<unsigned char>&)> &callback) {
	const auto writeDescriptorResult = mImpl->mCharacteristic.WriteClientCharacteristicConfigurationDescriptorAsync(GattClientCharacteristicConfigurationDescriptorValue::Notify).get();
	if (writeDescriptorResult != GattCommunicationStatus::Success)
		throw std::runtime_error("Unable set notification callback");

	mImpl->mCharacteristic.ValueChanged([=](const WinRTGattChar &, const GattValueChangedEventArgs &args) {
		auto dataReader = DataReader::FromBuffer(args.CharacteristicValue());
		std::vector<unsigned char> data(args.CharacteristicValue().Length());
		dataReader.ReadBytes(data);
		mImpl->mDataReceived.notifyAll(data);
	});

	return mImpl->mDataReceived.addListener(callback);
}

}
