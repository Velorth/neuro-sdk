#include "device/brainbit.h"

namespace Neuro {

std::vector<std::string> DeviceTraits<Brainbit>::validServiceUUIDs() { 
	return { "6E400001-B534-F393-68A9-E50E24DCCA9E" }; 
}

std::vector<std::string> DeviceTraits<Brainbit>::validNames() { 
	return { "BrainBit", "Brainbit" }; 
}

}