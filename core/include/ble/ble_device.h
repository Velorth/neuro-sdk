/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BLE_DEVICE_H
#define BLE_DEVICE_H

#include <string>
#include <functional>
#include "ble_device_info.h"
#include "common_types.h"

namespace Neuro {

enum class BleDeviceState : int {
    DISCONNECTED = 0,
    CONNECTED = 1,
    ERROR = 2
};

inline BleDeviceState parseBleDeviceState(int stateCode){
    switch (stateCode){
        case 0: return BleDeviceState::DISCONNECTED;
        case 1: return BleDeviceState::CONNECTED;
        case 2:
        default: return BleDeviceState::ERROR;
    }
}

enum class BleDeviceError : int {
    NO_ERROR = 0,
    TX_NOT_FOUND = 1,
    RX_NOT_FOUND = 2,
    STATUS_NOT_FOUND = 3,
    SERVICE_NOT_FOUND = 4,
    GENERAL_CONNECTION_ERROR = 5,
    UNKNOWN = 6
};

inline BleDeviceError parseBleErrorType(int errorCode){
    switch (errorCode){
        case 0: return BleDeviceError::NO_ERROR;
        case 1: return BleDeviceError::TX_NOT_FOUND;
        case 2: return BleDeviceError::RX_NOT_FOUND;
        case 3: return BleDeviceError::STATUS_NOT_FOUND;
        case 4: return BleDeviceError::SERVICE_NOT_FOUND;
        case 5: return BleDeviceError::GENERAL_CONNECTION_ERROR;
        default: return BleDeviceError::UNKNOWN;
    }
}

class BleDevice
{
public:
    using state_changed_callback_t = std::function<void(BleDeviceState, BleDeviceError)>;
    using data_received_callback_t = std::function<void(const std::vector<Byte> &)>;
    using status_received_callback_t = std::function<void(const std::vector<Byte> &)>;

    BleDevice(){}
    BleDevice(const BleDevice&) = delete;
    BleDevice& operator=(const BleDevice&) = delete;
    virtual ~BleDevice(){
        deviceStateChangedCallback = nullptr;
        dataReceivedCallback = nullptr;
        statusReceivedCallback = nullptr;
    }

    bool operator==(const BleDevice &other){
        return (this->getName() == other.getName()) &&
                (this->getNetAddress() == other.getNetAddress());
    }

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void close() = 0;
    virtual bool sendCommand(const std::vector<Byte> &commandData) = 0;
    virtual BleDeviceState getState() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getNetAddress() const = 0;

    DeviceType getDeviceType() const {
        return deviceInfo->getDeviceType();
    }

    void setStateChangedCallback(state_changed_callback_t callback){
        deviceStateChangedCallback = callback;
    }

    void setDataReceivedCallback(data_received_callback_t callback){
        dataReceivedCallback = callback;
    }

    void setStatusReceivedCallback(status_received_callback_t callback){
        statusReceivedCallback = callback;
    }

protected:
    std::unique_ptr<BleDeviceInfo> deviceInfo;
    state_changed_callback_t deviceStateChangedCallback;
    data_received_callback_t dataReceivedCallback;
    status_received_callback_t statusReceivedCallback;
};

}

#endif //BLE_DEVICE_H
