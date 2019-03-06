#ifndef CALLIBRI_H
#define CALLIBRI_H

#include <vector>
#include <string>
#include "device/device_info.h"

namespace Neuro {

class Callibri final {

};

template <>
struct DeviceTraits<Callibri> {
	SDK_SHARED static std::vector<std::string> validServiceUUIDs();
	SDK_SHARED static std::vector<std::string> validNames();
};

}
#endif // CALLIBRI_H
