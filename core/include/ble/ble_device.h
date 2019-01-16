/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neuromd.com/
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
#include "event_notifier.h"

namespace Neuro {

enum class BleDeviceState : int {
    Disconnected = 0,
    Connected = 1,
    Error = 2
};

inline BleDeviceState parseBleDeviceState(int stateCode){
    switch (stateCode){
        case 0: return BleDeviceState::Disconnected;
        case 1: return BleDeviceState::Connected;
        case 2:
        default: return BleDeviceState::Error;
    }
}

enum class BleDeviceError : int {
    NoError = 0,
    TxNotFound = 1,
    RxNotFound = 2,
    StatusNotFound = 3,
    ServiceNotFound = 4,
    GeneralConnectionError = 5,
    Unknown = 6
};

inline BleDeviceError parseBleErrorType(int errorCode){
    switch (errorCode){
        case 0: return BleDeviceError::NoError;
        case 1: return BleDeviceError::TxNotFound;
        case 2: return BleDeviceError::RxNotFound;
        case 3: return BleDeviceError::StatusNotFound;
        case 4: return BleDeviceError::ServiceNotFound;
        case 5: return BleDeviceError::GeneralConnectionError;
        default: return BleDeviceError::Unknown;
    }
}

class BleDevice
{
public:
    using state_changed_callback_t = std::function<void(BleDeviceState, BleDeviceError)>;
    using data_received_callback_t = std::function<void(const std::vector<Byte> &)>;
    using status_received_callback_t = std::function<void(const std::vector<Byte> &)>;

    BleDevice(std::unique_ptr<BleDeviceInfo> &&device_info):
        mDeviceInfo(std::move(device_info)){}
    BleDevice(const BleDevice&) = delete;
    BleDevice& operator=(const BleDevice&) = delete;
    virtual ~BleDevice(){
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

    std::shared_ptr<BleDeviceInfo> getGattInfo() const {
        return mDeviceInfo;
    }

    ListenerPtr<void, BleDeviceState, BleDeviceError> subscribeConnectionStateChanged(state_changed_callback_t callback){
        return mDeviceStateChangedCallback.addListener(callback);
    }

    ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(data_received_callback_t callback){
        return mDataReceivedCallback.addListener(callback);
    }

    ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(status_received_callback_t callback){
        return mStatusReceivedCallback.addListener(callback);
    }

protected:
    void notifyStateChanged(BleDeviceState state, BleDeviceError error){
        mDeviceStateChangedCallback.notifyAll(state, error);
    }

    void notifyDataReceived(const std::vector<Byte> &data){
        mDataReceivedCallback.notifyAll(data);
    }

    void notifyStatusReceived(const std::vector<Byte> &status){
        mStatusReceivedCallback.notifyAll(status);
    }

private:
    std::shared_ptr<BleDeviceInfo> mDeviceInfo;
    Notifier<void, BleDeviceState, BleDeviceError> mDeviceStateChangedCallback;
    Notifier<void, const std::vector<Byte> &> mDataReceivedCallback;
    Notifier<void, const std::vector<Byte> &> mStatusReceivedCallback;
};

}

#endif //BLE_DEVICE_H
