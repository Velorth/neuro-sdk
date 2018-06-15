#include "device_scanner/scanner_factory.h"

#ifdef __ANDROID__
    #include "ble/android/bluetooth_scanner.h"
    using CurrentPlatformScanner = Neuro::BluetoothScannerJni;
#elif __linux__
    #include "device_scanner_z.h"
    using CurrentPlatformScanner = Neuro::DeviceScannerZ;
#elif _WIN32
    #include "ble/win/ble_scanner_win.h"
    using CurrentPlatformScanner = Neuro::BleScannerWin;
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

}
#endif
