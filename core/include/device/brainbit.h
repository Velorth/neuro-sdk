#ifndef BRAINBIT_H
#define BRAINBIT_H

#include <vector>
#include <string>
#include "device/device_info.h"

namespace Neuro {

class Brainbit final {

};

template <>
struct DeviceTraits<Brainbit> {
	SDK_SHARED static std::vector<std::string> validServiceUUIDs();
	SDK_SHARED static std::vector<std::string> validNames();
};

}
#endif // BRAINBIT_H
