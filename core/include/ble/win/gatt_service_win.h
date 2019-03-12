#ifndef GATT_SERVICE_WIN_H
#define GATT_SERVICE_WIN_H

#include <memory>
#include <vector>
#include "ble/win/gatt_characteristic_win.h"

namespace Neuro {

class GattServiceWin final {
public:
	explicit GattServiceWin(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService);
	GattServiceWin(const GattServiceWin &) = delete;
	GattServiceWin& operator=(const GattServiceWin &) = delete;
	GattServiceWin(GattServiceWin &&) noexcept;
	GattServiceWin& operator=(GattServiceWin &&) noexcept;
	~GattServiceWin();

	GUID uuid() const;
	std::vector<GattCharacteristicWin> characteristics() const;
private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}

#endif // GATT_SERVICE_WIN_H
