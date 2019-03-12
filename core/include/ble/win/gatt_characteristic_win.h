#ifndef GATT_CHARACTERISTIC_WIN_H
#define GATT_CHARACTERISTIC_WIN_H

#include <memory>
#include <vector>
#include "event_listener.h"
#include "winrt/Windows.Devices.Bluetooth.GenericAttributeProfile.h"

namespace Neuro {

class GattCharacteristicWin final {
public:
	explicit GattCharacteristicWin(winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic);
	GattCharacteristicWin(const GattCharacteristicWin &) = delete;
	GattCharacteristicWin& operator=(const GattCharacteristicWin &) = delete;
	GattCharacteristicWin(GattCharacteristicWin &&) noexcept;
	GattCharacteristicWin& operator=(GattCharacteristicWin &&) noexcept;
	~GattCharacteristicWin();

	GUID uuid() const;
	std::vector<unsigned char> read() const;
	void write(const std::vector<unsigned char> &);
	ListenerPtr<void, const std::vector<unsigned char> &>
		subscribeCharacteristicChanged(const std::function<void(const std::vector<unsigned char> &)> &);
private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}

#endif // GATT_CHARACTERISTIC_WIN_H
