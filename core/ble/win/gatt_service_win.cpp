#include "ble/win/gatt_service_win.h"

using winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService;

namespace Neuro {

struct GattServiceWin::Impl {
	Impl(GattDeviceService service):
		mService(service){}

	~Impl() {
		try {
			mService.Close();
		}
		catch (...) {}
	}

	GattDeviceService mService;
};

GattServiceWin::GattServiceWin(GattDeviceService service):
	mImpl(std::make_unique<Impl>(service)){	
}

GattServiceWin::GattServiceWin(GattServiceWin&&) noexcept = default;
GattServiceWin& GattServiceWin::operator=(GattServiceWin&&) noexcept = default;

GattServiceWin::~GattServiceWin() = default;

GUID GattServiceWin::uuid() const {
	return mImpl->mService.Uuid();
}

std::vector<GattCharacteristicWin> GattServiceWin::characteristics() const {
	auto result = mImpl->mService.GetCharacteristicsAsync().get().Characteristics();
	std::vector<GattCharacteristicWin> characteristics;
	std::transform(begin(result), end(result), std::back_inserter(characteristics), [](const auto &characteristic) { return GattCharacteristicWin(characteristic); });
	return characteristics;
}

}
