#ifndef GATT_CHARACTERISTIC_MAC_H
#define GATT_CHARACTERISTIC_MAC_H

#include <memory>
#include <vector>
#include "event_listener.h"
#include <CoreBluetooth/CoreBluetooth.h>

namespace Neuro {

class GattCharacteristicMac final {
public:
	explicit GattCharacteristicMac(CBCharacteristic*);
	GattCharacteristicMac(const GattCharacteristicMac &) = delete;
	GattCharacteristicMac& operator=(const GattCharacteristicMac &) = delete;
	GattCharacteristicMac(GattCharacteristicMac &&) noexcept;
	GattCharacteristicMac& operator=(GattCharacteristicMac &&) noexcept;
	~GattCharacteristicMac();

	CBUUID uuid() const;
	std::vector<unsigned char> read() const;
	void write(const std::vector<unsigned char> &);
	ListenerPtr<void, const std::vector<unsigned char> &>
		subscribeCharacteristicChanged(const std::function<void(const std::vector<unsigned char> &)> &);
private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}

#endif // GATT_CHARACTERISTIC_MAC_H
