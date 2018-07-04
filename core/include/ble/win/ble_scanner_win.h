#ifndef BLE_SCANNER_WIN
#define BLE_SCANNER_WIN

#include <atomic>
#include <memory>
#include <string>
#include <set>
#include "ble/ble_scanner.h"
#include "loop.h"
#include "task_queue.h"

namespace Neuro {

class BleScannerWin : public BleScanner{
public:
    BleScannerWin();
    BleScannerWin(const BleScannerWin &) = delete;
    BleScannerWin& operator=(const BleScannerWin &) = delete;

    void startScan() override;
    void stopScan() override;
    std::unique_ptr<BleDevice> getDeviceByAddress(std::string) override;
    void setFilter(std::vector<std::string>) override;
    void subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)>) override;
    bool isScanning() override;
    void releaseDevice(std::string name, std::string address) override;

private:
    static constexpr const char *class_name = "BleScannerWin";

    std::atomic<bool> mIsScanning{false};
    std::vector<std::string> mFilterCollection;
    std::set<std::string> mFoundDeviceAddresses;
    std::function<void(std::unique_ptr<BleDevice>)> mDeviceFoundCallback;
    TaskQueue mScanTaskQueue{"ScanTaskQueue"};
    Loop<void(BleScannerWin*)> mSpawnScanTaskLoop;

    void spawnScanTask();
    std::vector<std::unique_ptr<BleDevice>> findDevicesWithFilters(const std::vector<std::string> &);
    void onDeviceFound(std::unique_ptr<BleDevice> &&);
};

}

#endif
