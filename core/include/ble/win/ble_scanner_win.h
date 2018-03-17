#ifndef BLE_SCANNER_WIN
#define BLE_SCANNER_WIN

#include "ble/ble_scanner.h"

namespace Neuro {

class BleScannerWin : public BleScanner{
public:
    void startScan() override;
    void stopScan() override;
    std::unique_ptr<BleDevice> getDeviceByAddress(std::string) override;
    void setFilter(std::vector<std::string>) override;
    void subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)>) override;
    bool isScanning() override;
    void releaseDevice(std::string name, std::string address) override;
};

}

#endif
