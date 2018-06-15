#include "ble/ios/ble_scanner_objc.h"
#include "device_scanner/scanner_factory.h"

namespace Neuro {

std::unique_ptr<DeviceScanner> createDeviceScanner(){
    auto bleScanner = std::make_unique<NCBleScanner>();
    auto deviceScanner = std::make_unique<DeviceScanner>(std::move(bleScanner));
    return deviceScanner;
}

}
#endif
