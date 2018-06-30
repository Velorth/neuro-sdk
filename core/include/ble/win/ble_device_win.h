#ifndef BLE_DEVICE_WIN
#define BLE_DEVICE_WIN

#include "ble/ble_device.h"
#include "ble/win/gatt_wrapper.h"

namespace Neuro {

class BleDeviceWin : public BleDevice{
public:
    BleDeviceWin(DeviceHandle &&, std::string name, std::string address);
    void connect() override;
    void disconnect() override;
    void close() override;
    bool sendCommand(const std::vector<Byte> &commandData) override;
    BleDeviceState getState() const override;
    std::string getName() const override;
    std::string getNetAddress() const override;

private:
    DeviceHandle mDeviceHandle;
    std::string mName;
    std::string mAddress;
};

}

#endif
