#ifndef GATT_SERVICE_MAC_H
#define GATT_SERVICE_MAC_H

#include <memory>
#include <vector>
#include "ble/mac/gatt_characteristic_mac.h"

namespace Neuro {

class GattServiceMac final {
public:
	explicit GattServiceMac(CBService*);
	GattServiceMac(const GattServiceMac &) = delete;
	GattServiceMac& operator=(const GattServiceMac &) = delete;
	GattServiceMac(GattServiceMac &&) noexcept;
	GattServiceMac& operator=(GattServiceMac &&) noexcept;
	~GattServiceMac();

	CDUUID uuid() const;
	std::vector<GattCharacteristicMac> characteristics() const;
private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}

#endif // GATT_SERVICE_MAC_H
