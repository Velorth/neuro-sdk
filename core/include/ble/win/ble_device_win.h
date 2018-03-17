#ifndef BLE_DEVICE_WIN
#define BLE_DEVICE_WIN

#include "ble/ble_device.h"

namespace Neuro {

class BleDeviceWin : public BleDevice{
public:
    void connect() override;
    void disconnect() override;
    void close() override;
    bool sendCommand(const std::vector<Byte> &commandData) override;
    BleDeviceState getState() const override;
    std::string getName() const override;
    std::string getNetAddress() const override;
};

}

#endif
