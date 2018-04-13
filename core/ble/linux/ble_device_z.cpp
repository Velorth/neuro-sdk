#include "ble_device_z.h"
#include <thread>
#include <functional>
#include "logger.h"

using std::string;
using std::shared_ptr;



BleDeviceZ::BleDeviceZ(string name, string netAddress):
    name(name),
    netAddress(netAddress),
    connection(nullptr),
    rxUUID({0, 0}),
    txUUID({0, 0}),
    statusUUID({0, 0}),
    dataThreadStop(false),
    receivedDataHndlThread(&BleDeviceZ::receivedDataHndlFunc, this),
    statusThreadStop(false),
    receivedStatusHndlThread(&BleDeviceZ::receivedStatusHndlFunc, this)
{
    deviceInfo = DeviceInfo::fromDeviceName(name);
}

BleDeviceZ::~BleDeviceZ()
{
    close();

    dataThreadStop.store(true);
    if (receivedDataHndlThread.joinable())
        receivedDataHndlThread.join();

    statusThreadStop.store(true);
    if (receivedStatusHndlThread.joinable())
        receivedStatusHndlThread.join();
}

void BleDeviceZ::connect()
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Start connect", "BleDeviceZ", __FUNCTION__);
        std::unique_lock<std::mutex> cLock(connectionMutex);
        if(connection != nullptr)
        {
            log->debug("[%s: %s] Connection is not null", "BleDeviceZ", __FUNCTION__);
           return;
        }

        log->debug("[%s: %s] Call to gattlib_connect", "BleDeviceZ", __FUNCTION__);

        gattlib_connect_async(nullptr, netAddress.c_str(), BDADDR_LE_RANDOM, BT_SEC_LOW, 0, 0, &BleDeviceZ::onGattConnected, this);
}

void BleDeviceZ::onGattConnected(gatt_connection_t *connection, void* user_data)
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto bleDevice = static_cast<BleDeviceZ *>(user_data);

    bleDevice->connection = connection;
    if(bleDevice->connection == nullptr)
    {
        log->warn("[%s: %s] Connection is null after gattlib_connect", "BleDeviceZ", __FUNCTION__);
        bleDevice->state = BleDeviceState::ERROR;
        bleDevice->onDeviceStateChanged(bleDevice->state, BleDeviceError::GENERAL_CONNECTION_ERROR);
        return;
    }

    log->debug("[BleDeviceZ:%s] Gatt connected", __FUNCTION__);

    if(!bleDevice->gattDicover() // if has error dicover
            || !bleDevice->invokeReady() // if has error invoke ready (for example rx not found)
            || !bleDevice->registerNotification()) // if has register notification
    {
        log->debug("[%s: %s] Gatt discover failure. Disconnecting...", "BleDeviceZ", __FUNCTION__);
        bleDevice->state = BleDeviceState::ERROR;
        bleDevice->onDeviceStateChanged(bleDevice->state, BleDeviceError::GENERAL_CONNECTION_ERROR);
    }
    else{
        bleDevice->state = BleDeviceState::CONNECTED;
        std::thread([bleDevice](){
            bleDevice->onDeviceStateChanged(bleDevice->state, BleDeviceError::NO_ERROR);
        }).detach();
    }
}

bool BleDeviceZ::gattDicover()
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Discovering characteristics", "BleDeviceZ", __FUNCTION__);
    if(gattInfoReady)
        return true;
    if(gattServiceIsProvided() != 1)
    {
        log->error("[%s: %s] Device main service not found", "BleDeviceZ", __FUNCTION__);
        state = BleDeviceState::ERROR;
        onDeviceStateChanged(state, BleDeviceError::SERVICE_NOT_FOUND);
        return false;
    }

    if(gattCharacteristicIsProvided() != 1)
    {
        log->error("[%s: %s] Device characteristics not found", "BleDeviceZ", __FUNCTION__);
        state = BleDeviceState::ERROR;
        onDeviceStateChanged(state, BleDeviceError::SERVICE_NOT_FOUND);; // Characteristic check error
        return false;
    }
    log->debug("[BleDeviceZ:%s] On gatt discover TRUE", __FUNCTION__);
    return (gattInfoReady = true);
}


char BleDeviceZ::gattServiceIsProvided()
{
    gattlib_primary_service_t* services;
    int services_count;
    int ret, i;
    ret = gattlib_discover_primary(connection, &services, &services_count);
    if (ret != 0)
        return 2; // Fail to discover primary services
    bool serviceProvided = false;
    uuid_t deviceServiceUUID;
    gattlib_string_to_uuid(deviceInfo->getGattInfo()->deviceServiceUUID().c_str(),
                           deviceInfo->getGattInfo()->deviceServiceUUID().length(),
                           &deviceServiceUUID);
    for (i = 0; i < services_count; i++)
    {
        if(gattlib_uuid_cmp(&deviceServiceUUID, &services[i].uuid) == 0)
        {
            serviceProvided = true;
            break;
        }
    }
    free(services);
    return (serviceProvided) ? 1 : 0; // 1 - ok, 2 - otherwise
}

char BleDeviceZ::gattCharacteristicIsProvided()
{
    gattlib_characteristic_t* characteristics;
    int characteristics_count;
    int ret, i;
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Call to gattlib_discover", "BleDeviceZ", __FUNCTION__);
    ret = gattlib_discover_char(connection, &characteristics, &characteristics_count);
    if (ret != 0){
        log->error("[%s: %s] Failed discover services", "BleDeviceZ", __FUNCTION__);
        return 2; // Fail to discover characteristics
    }

    log->debug("[%s: %s] Service discovered", "BleDeviceZ", __FUNCTION__);
    uuid_t rxCharacteristicUUID = {0, 0};
    uuid_t txCharacteristicUUID = {0, 0};
    uuid_t statusCharacteristicUUID = {0, 0};

    if(!deviceInfo->getGattInfo()->rxCharacteristicUUID().empty())
        gattlib_string_to_uuid(deviceInfo->getGattInfo()->rxCharacteristicUUID().c_str(),
                               deviceInfo->getGattInfo()->rxCharacteristicUUID().length(),
                               &rxCharacteristicUUID);
    if(!deviceInfo->getGattInfo()->txCharacteristicUUID().empty())
        gattlib_string_to_uuid(deviceInfo->getGattInfo()->txCharacteristicUUID().c_str(),
                               deviceInfo->getGattInfo()->txCharacteristicUUID().length(),
                               &txCharacteristicUUID);
    if(!deviceInfo->getGattInfo()->statusCharacteristicUUID().empty())
        gattlib_string_to_uuid(deviceInfo->getGattInfo()->statusCharacteristicUUID().c_str(),
                               deviceInfo->getGattInfo()->statusCharacteristicUUID().length(),
                               &statusCharacteristicUUID);

    log->debug("[%s: %s] Checking characteristics uuids", "BleDeviceZ", __FUNCTION__);
    for (i = 0; i < characteristics_count; i++)
    {
        if(gattlib_uuid_cmp(&rxCharacteristicUUID, &characteristics[i].uuid) == 0)
            rxUUID = characteristics[i].uuid;
        else if(gattlib_uuid_cmp(&txCharacteristicUUID, &characteristics[i].uuid) == 0)
            txUUID = characteristics[i].uuid;
        else if(gattlib_uuid_cmp(&statusCharacteristicUUID, &characteristics[i].uuid) == 0)
            statusUUID = characteristics[i].uuid;
    }
    free(characteristics);

    log->debug("[%s: %s] %d characteristicsfound", "BleDeviceZ", __FUNCTION__, characteristics_count);
    return (characteristics_count > 0) ? 1 : 0;
}

void BleDeviceZ::disconnect()
{
    std::unique_lock<std::mutex> cLock(connectionMutex);
    if(connection != nullptr)
    {
        auto log = LoggerFactory::getCurrentPlatformLogger();
        log->debug("[%s: %s] Call to gattlib_disconnect", "BleDeviceZ", __FUNCTION__);
        gattlib_disconnect(connection);
        log->debug("[%s: %s] Gatt disconnected", "BleDeviceZ", __FUNCTION__);
        connection = nullptr;
        state = BleDeviceState::DISCONNECTED;
        onDeviceStateChanged(state, BleDeviceError::NO_ERROR);
    }
}

void BleDeviceZ::close()
{
    disconnect();
    rxUUID = {0, 0};
    txUUID = {0, 0};
    statusUUID = {0, 0};
    gattInfoReady = false;
}

bool BleDeviceZ::sendCommand(const std::vector<Byte> &commandData)
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Sending command", "BleDeviceZ", __FUNCTION__);
    if(txUUID.type == 0)
        return false;

    std::unique_lock<std::mutex> cLock(connectionMutex);
    if(connection == nullptr)
        return false;
    auto result = !gattlib_write_char_by_uuid(connection, &txUUID, commandData.data(), commandData.size());
    log->debug("[%s] Send command result: %s", __FUNCTION__, result?"SUCCESS":"FAILURE");
    return result;
}

string BleDeviceZ::getName() const
{
    return name;
}

string BleDeviceZ::getNetAddress() const
{
    return netAddress;
}

BleDeviceState BleDeviceZ::getState() const {
    return state;
}

void BleDeviceZ::deviceNotificationHandler(const uuid_t* uuid, const uint8_t* data, size_t data_length, void* user_data)
{
    if(user_data == nullptr)
        return;
    BleDeviceZ * bleDevice = static_cast<BleDeviceZ *>(user_data);
    if(gattlib_uuid_cmp(&bleDevice->rxUUID, uuid) == 0) // Data recived
    {        
        bleDevice->pushReceivedData(std::vector<uint8_t>(data, data + data_length));
    }
    else if(gattlib_uuid_cmp(&bleDevice->statusUUID, uuid) == 0) // Status recived
    {
        bleDevice->pushReceivedStatus(std::vector<uint8_t>(data, data + data_length));
    }
}

bool BleDeviceZ::registerNotification()
{
     auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[BleDeviceZ:%s] Starting notifications...", __FUNCTION__);
    gattlib_register_notification(connection, BleDeviceZ::deviceNotificationHandler, this);

    if (rxUUID.type != 0
            && gattlib_notification_start(connection, &rxUUID) != 0) // Fail to start notification RX
    {
        state = BleDeviceState::ERROR;
        onDeviceStateChanged(state, BleDeviceError::RX_NOT_FOUND);
        return false;
    }

    if (statusUUID.type != 0
            && gattlib_notification_start(connection, &statusUUID) != 0) // Fail to start notification status
    {
        state = BleDeviceState::ERROR;
        onDeviceStateChanged(state, BleDeviceError::STATUS_NOT_FOUND);
        return false;
    }
    log->debug("[BleDeviceZ:%s] Notifications started successfully", __FUNCTION__);
    return true;
}

bool BleDeviceZ::invokeReady()
{
     auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[BleDeviceZ:%s] Invoke ready check func", __FUNCTION__);
    if(!rxUUID.type)
    {
        state = BleDeviceState::ERROR;
        onDeviceStateChanged(state, BleDeviceError::RX_NOT_FOUND);
        return false;
    }
    if(txUUID.type != 0 && (!deviceInfo->getGattInfo()->statusCharacteristicUUID().empty() == (statusUUID.type != 0))){
        log->debug("[BleDeviceZ:%s] On device connection ready", __FUNCTION__);
        return true;
    }
    return false;
}

void BleDeviceZ::pushReceivedData(std::vector<uint8_t> &&data)
{
    std::unique_lock<std::mutex> enqueueLock(receivedDataQueueMutex);
    receivedDataQueue.push(std::move(data));
    emptyDataQueueCondition.notify_all();
}

void BleDeviceZ::pushReceivedStatus(std::vector<uint8_t> &&status)
{
    std::unique_lock<std::mutex> enqueueLock(receivedStatusQueueMutex);
    receivedStatusQueue.push(std::move(status));
    emptyStatusQueueCondition.notify_all();
}

void BleDeviceZ::receivedDataHndlFunc()
{
    while(!dataThreadStop.load()){
        std::unique_lock<std::mutex> queueEmptyLock(receivedDataQueueMutex);
        if (!receivedDataQueue.size()){
            emptyDataQueueCondition.wait(queueEmptyLock);
        }

        auto queuedData = std::move(receivedDataQueue.front());
        receivedDataQueue.pop();
        queueEmptyLock.unlock();
        onDataReceived(queuedData);
    }
}

void BleDeviceZ::receivedStatusHndlFunc()
{
    while(!statusThreadStop.load()){
        std::unique_lock<std::mutex> queueEmptyLock(receivedStatusQueueMutex);
        if (!receivedStatusQueue.size()){
            emptyStatusQueueCondition.wait(queueEmptyLock);
        }

        auto queuedData = std::move(receivedStatusQueue.front());
        receivedStatusQueue.pop();
        queueEmptyLock.unlock();

        onStatusReceived(queuedData);
    }
}
