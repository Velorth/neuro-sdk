#ifndef BLE_SCANNER_WIN
#define BLE_SCANNER_WIN

#include <atomic>
#include <memory>
#include <string>
#include <set>
#include "ble/ble_scanner.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"

namespace Neuro {

class BleScannerWin : public BleScanner{
public:
	BleScannerWin();
    BleScannerWin(const BleScannerWin &) = delete;
    BleScannerWin& operator=(const BleScannerWin &) = delete;
	~BleScannerWin();

    void startScan() override;
    void stopScan() override;
    std::unique_ptr<BleDevice> getDeviceByAddress(std::string) override;
    void setFilter(std::vector<std::string>) override;
    void subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)>) override;
    bool isScanning() override;
    void releaseDevice(std::string name, std::string address) override;

private:
    static constexpr const char *class_name = "BleScannerWin";
	using BleWatcherType = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;

	BleWatcherType mWatcher;
	winrt::event_token mReceivedEventToken;
	winrt::event_token mStoppedEventToken;
    std::atomic<bool> mIsScanning{false};
    std::vector<std::string> mFilterCollection;    
	std::set<unsigned long long> mFoundDeviceAddresses;
    std::function<void(std::unique_ptr<BleDevice>)> mDeviceFoundCallback;

	void onAdvertisementReceived(const BleWatcherType &watcher, const winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs &args);
    void onDeviceFound(std::unique_ptr<BleDevice> &&) const;
};

}

#endif
