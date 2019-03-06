#include "ble/win/ble_device_address.h"
#include <iostream>

 std::ostream & Neuro::operator<<(std::ostream & out_stream, const BleDeviceAddress & address) {
	 return out_stream << std::hex << static_cast<uint64_t>(address);
 }
