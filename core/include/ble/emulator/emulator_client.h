#ifndef EMULATOR_CLIENT_H
#define EMULATOR_CLIENT_H

#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace Neuro {

class BleDevice;

class EmulatorClient {
public:
    EmulatorClient();
    EmulatorClient(const EmulatorClient &) = delete;
    EmulatorClient& operator=(const EmulatorClient &) = delete;
    ~EmulatorClient();

    void startScan();
    void stopScan();
    std::unique_ptr<BleDevice> getDeviceByAddress(std::string);
    void setFilter(std::vector<std::string>);
    void subscribeDeviceFound(std::function<void(std::unique_ptr<BleDevice>)>);
    bool isScanning();
    void releaseDevice(std::string name, std::string address);

private:
    std::atomic<bool> scanning{false};
    std::function<void (std::unique_ptr<BleDevice>)> deviceFoundCallback;
    std::vector<std::string> deviceFilter;
    std::vector<std::string> foundAddresses;
};

}

#endif // EMULATOR_CLIENT_H
