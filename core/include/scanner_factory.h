#ifndef SCANNER_FACTORY_H
#define SCANNER_FACTORY_H

#include "device_scanner.h"
#include <memory>

#ifdef __ANDROID__
    #include "ble/android/bluetooth_scanner.h"
    using CurrentPlatformScanner = Neuro::BluetoothScannerJni;
#elif __linux__
    #include "device_scanner_z.h"
    using CurrentPlatformScanner = DeviceScannerZ;
#elif _WIN32
    #include "ble_scanner_win.h"
    using CurrentPlatformScanner = BleScannerWin;
#elif __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_IOS
        #include "ios/ble_scanner_objc.h"
        using CurrentPlatformScanner = NCBleScanner;
    #else
        #error "Current APPLE platform is not supported"
#endif
#else
    #error "Unsupported platform"
#endif

namespace Neuro {

#ifdef __ANDROID__
std::unique_ptr<DeviceScanner> createDeviceScanner(jobject context){
    auto bleScanner = std::make_unique<CurrentPlatformScanner>(context);
    auto deviceScanner = std::make_unique<DeviceScanner>(std::move(bleScanner));
    return deviceScanner;
}
#else
std::unique_ptr<DeviceScanner> createDeviceScanner(){
    auto bleScanner = std::make_unique<CurrentPlatformScanner>();
    auto deviceScanner = std::make_unique<DeviceScanner>(std::move(bleScanner));
    return deviceScanner;
}
#endif

}

#endif // SCANNER_FACTORY_H
