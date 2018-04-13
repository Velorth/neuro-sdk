#ifndef EMULATED_DEVICE_H
#define EMULATED_DEVICE_H

#include "ble_device.h"
#include "socket.h"

namespace Neuro {

class EmulatedDevice : public BleDevice {
public:
    EmulatedDevice(std::string name, std::string address);
    void connect() override;
    void disconnect() override;
    void close() override;
    bool sendCommand(const std::vector<Byte> &commandData) override;
    BleDeviceState getState() const override;
    std::string getName() const override;
    std::string getNetAddress() const override;

private:
    std::string deviceName;
    std::string deviceAddress;
    BleDeviceState mState{BleDeviceState::DISCONNECTED};
    unsigned short mPort;
    unsigned short sendPort;
    std::unique_ptr<Net::UdpSocket> deviceSocket{nullptr};
};

}

#endif // EMULATED_DEVICE_H
