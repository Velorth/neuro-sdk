#ifndef BLE_DEVICE_ADDRESS_H
#define BLE_DEVICE_ADDRESS_H

#include <iosfwd>
#include <cstdint>
#include <functional>
#include <string>
#include "lib_export.h"

namespace Neuro {
struct BleDeviceAddress final {
	BleDeviceAddress() = default;

	explicit BleDeviceAddress(uint64_t value) :
		mValue(value) {}

	explicit operator uint64_t() const {
		return mValue;
	}

private:
	uint64_t mValue;
};

inline bool operator==(const BleDeviceAddress &lhs, const BleDeviceAddress &rhs) {
	return static_cast<uint64_t>(lhs) == static_cast<uint64_t>(rhs);
}

SDK_SHARED std::ostream& operator<<(std::ostream& out_stream, const BleDeviceAddress &address);

SDK_SHARED std::string to_string(const BleDeviceAddress &);

}

template <>
struct std::hash<Neuro::BleDeviceAddress> {
	size_t operator()(const Neuro::BleDeviceAddress &address) const noexcept {
		using std::hash;
		return hash<uint64_t>()(static_cast<uint64_t>(address));
	}
};

#endif // BLE_DEVICE_ADDRESS_H
