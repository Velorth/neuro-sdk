#include "ble/win/ble_device_address.h"
#include <iostream>
#include <sstream>

 std::ostream & Neuro::operator<<(std::ostream & out_stream, const BleDeviceAddress & address) {
	 return out_stream << std::hex << static_cast<uint64_t>(address);
 }

std::string Neuro::to_string(const BleDeviceAddress &address) {
	std::stringstream resultStream;
	resultStream << address;
	return resultStream.str();
 }
