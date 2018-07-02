#ifndef BLE_DEVICE_WIN
#define BLE_DEVICE_WIN

#include "ble/ble_device.h"
#include "ble/win/gatt_wrapper.h"
#include "loop.h"
#include "task_queue.h"

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
    static constexpr const char *class_name = "BleDeviceWin";

    DeviceHandle mDeviceHandle;
    std::string mName;
    std::string mAddress;
    BTH_LE_GATT_SERVICE mService;
    BTH_LE_GATT_CHARACTERISTIC mRxCharacteristic;
    BTH_LE_GATT_CHARACTERISTIC mTxCharacteristic;
    std::atomic<bool> mIsConnected{false};
    TaskQueue mGetServiceTaskQueue;
    //Loop<void()> mTaskSpawnLoop;

    void spawnGetServiceTask();
};

}

#endif
