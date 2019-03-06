#include "device/device_info.h"
#include <iostream>

std::ostream& Neuro::operator<<(std::ostream& out_stream, const DeviceInfo& info) {
	return out_stream << info.Name << " [" << info.Address << "]";
 }
