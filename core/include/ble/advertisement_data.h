#ifndef ADVERTISEMENT_DATA_H
#define ADVERTISEMENT_DATA_H

#include <string>
#include <vector>
#include "platform_traits_impl.h"

namespace Neuro {

struct AdvertisementData final {
	std::string Name;
	DeviceAddressType Address;
	int RSSI;
	std::vector<UUIDType> ServicesUUIDs;
	BleTimeType TimeStamp;
};

}

#endif // ADVERTISEMENT_DATA_H
