#include "device_scanner/scanner_factory.h"

namespace Neuro {

std::unique_ptr<DeviceScanner> createDeviceScanner(){
    throw std::runtime_error("DeviceScanner is deprecated");
}

}
