#ifndef BLUETOOTHDEVICEZ_H
#define BLUETOOTHDEVICEZ_H

#include "device/ble_device.h"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <gattlib.h>

class BleDeviceZ: public BleDevice
{
public:
    BleDeviceZ(std::string name, std::string netAddress);
    BleDeviceZ();
    ~BleDeviceZ();

    void connect() override;
    void disconnect() override;
    void close() override;
    bool sendCommand(const std::vector<Byte> &commandData) override;
    std::string getName() const override;
    std::string getNetAddress() const override;
    BleDeviceState getState() const override;

private:
    std::string name;
    std::string netAddress;
    gatt_connection_t* connection;
    bool gattInfoReady;

    uuid_t rxUUID;
    uuid_t txUUID;
    uuid_t statusUUID;

    bool gattDicover();
    char gattServiceIsProvided();
    char gattCharacteristicIsProvided();
    bool registerNotification();
    bool invokeReady();    
    void pushReceivedData(std::vector<uint8_t> &&);
    void pushReceivedStatus(std::vector<uint8_t> &&);
    void receivedDataHndlFunc();
    void receivedStatusHndlFunc();

    BleDeviceState state{BleDeviceState::DISCONNECTED};
    std::queue<std::vector<uint8_t>> receivedDataQueue;
    std::mutex receivedDataQueueMutex;
    std::queue<std::vector<uint8_t>> receivedStatusQueue;
    std::mutex receivedStatusQueueMutex;
    std::mutex connectionMutex;
    std::atomic<bool> dataThreadStop;
    std::condition_variable emptyDataQueueCondition;
    std::thread receivedDataHndlThread;
    std::atomic<bool> statusThreadStop;
    std::condition_variable emptyStatusQueueCondition;
    std::thread receivedStatusHndlThread;

    static void onGattConnected(gatt_connection_t* connection, void* user_data);
    static void deviceNotificationHandler(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data);
};
#endif // BLUETOOTHDEVICEZ_H
