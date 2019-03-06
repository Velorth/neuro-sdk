#include "device/callibri.h"

namespace Neuro {

std::vector<std::string> DeviceTraits<Callibri>::validServiceUUIDs() {
	return {
		"3D2F0001-D6B9-11E4-88CF-0002A5D5C51B",
		"67CF0001-FA71-11E5-80B7-0002A5D5C51B",
		"77FF0001-FA66-11E5-B501-0002A5D5C51B",
		"B9390001-FA71-11E5-A787-0002A5D5C51B"
	};
}

std::vector<std::string> DeviceTraits<Callibri>::validNames() {
	return {
		"Neurotech_Colibri_R",
		"Neurotech_Callibri_R",
		"Callibri_Red",
		"Neurotech_Colibri_B",
		"Neurotech_Callibri_B",
		"Callibri_Blue",
		"Neurotech_Colibri_Y",
		"Neurotech_Callibri_Y",
		"Callibri_Yellow",
		"Neurotech_Colibri_W",
		"Neurotech_Callibri_W",
		"Callibri_White"
	};
}
};

