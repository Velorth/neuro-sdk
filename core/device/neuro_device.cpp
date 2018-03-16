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

#include <functional>
#include "device/neuro_device.h"
#include "device/brainbit_device.h"
#include "device/colibri_device.h"

using std::shared_ptr;
using std::vector;

/*
Device::Device(std::unique_ptr<BleDevice> bleDev):
        bleDevice(std::move(bleDev)),
        deviceState(DeviceState::UNKNOWN),
        deviceStateError(DeviceError::NO_ERROR),
        isInitialized(false) {}

Device::~Device(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Neuro device destructor. Device %s, address: %s", "Device", __FUNCTION__, getName().c_str(), getAddress().c_str());
    bleDevice->disconnect();
    bleDevice->close();
    log->debug("[%s: %s] Neuro device destructor EXIT", "Device", __FUNCTION__);
}

void Device::disconnect(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Ble disconnect. Device %s, address: %s", "Device", __FUNCTION__,
               getName().c_str(), getAddress().c_str());
    pendingDisconnectRequest = true;
    bleDevice->disconnect();
}

void Device::close() {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Ble close. Device %s, address: %s", "Device", __FUNCTION__,
               getName().c_str(), getAddress().c_str());
    pendingDisconnectRequest = true;
    bleDevice->close();
}

void Device::connect(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Ble connect. Device %s, address: %s", "Device", __FUNCTION__,
               getName().c_str(), getAddress().c_str());
    if (!bleDeviceStateHandler) {
        subscribeBleDeviceStateChanged();
    }
    if (!statusReceivedHandler){
        subscribeStatusReceived();
    }
    if (!dataReceivedHandler){
        subscribeDataReceived();
    }
    bleDevice->connect();
}

DeviceState Device::getState() const {

    return deviceState;
}

DeviceError Device::getError() const {

    return deviceStateError;
}

vector<DeviceFeature> Device::getFeatures() const {

    return deviceFeatures;
}

std::string Device::getName() const {

    return bleDevice->getName();
}

std::string Device::getAddress() const {

    return bleDevice->getNetAddress();
}

std::vector<Neuro::ChannelInfo> Device::availableChannels() const noexcept {
    return mAvailableChannels;
}

void Device::notifyDeviceStateChanged(DeviceState state, DeviceError error) {
    deviceStateChanged(*this, state, error);
}

void Device::notifyBatteryStateChanged(int batteryLevel, bool isCharging) {
    batteryStateChanged(*this, batteryLevel, isCharging);
}

void Device::onBleDeviceStateChanged(BleDeviceState state, BleDeviceError error) {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Ble device state: %d. Device %s, address: %s", "Device", __FUNCTION__, static_cast<int>(state),
               getName().c_str(), getAddress().c_str());
    if (state == BleDeviceState::CONNECTED) {
        log->debug("[%s: %s] Ble device connected. Device %s, address: %s. Initializing...", "Device", __FUNCTION__,
                   getName().c_str(), getAddress().c_str());
        pendingDisconnectRequest = false;
        if (initialize()) {
            deviceState = DeviceState::CONNECTED;
            deviceStateError = DeviceError::NO_ERROR;
            log->debug("[%s: %s] Neuro device connected. Device %s, address: %s", "Device", __FUNCTION__,
                       getName().c_str(), getAddress().c_str());
        }
        else{
            deviceState = DeviceState::ERROR;
            deviceStateError = DeviceError::PROTOCOL_ERROR;
            log->error("[%s: %s] Neuro device protocol error. Device %s, address: %s", "Device", __FUNCTION__,
                       getName().c_str(), getAddress().c_str());
            bleDevice->disconnect();
        }
    }
    else if (state == BleDeviceState::DISCONNECTED) {
        deviceState = DeviceState::DISCONNECTED;
        deviceStateError = DeviceError::NO_ERROR;
        isInitialized = false;
        log->debug("[%s: %s] Neuro device disconnected. Device %s, address: %s", "Device", __FUNCTION__,
                   getName().c_str(), getAddress().c_str());
        if (!pendingDisconnectRequest){
            connect();
        }
    }
    else {
        deviceState = DeviceState::ERROR;
        deviceStateError = DeviceError::BLUETOOTH_CONNECTION_ERROR;
        isInitialized = false;
        log->error("[%s: %s] Neuro device bluetooth error. Device %s, address: %s", "Device", __FUNCTION__,
                   getName().c_str(), getAddress().c_str());
    }
    notifyDeviceStateChanged(deviceState, deviceStateError);
}

void Device::subscribeBleDeviceStateChanged() {
    std::weak_ptr<Device> neuroDevicePtr = shared_from_this();
    bleDeviceStateHandler = MakeHandler(BleDevice, deviceStateChanged,
                                        [neuroDevicePtr](const BleDevice &, BleDeviceState state,
                                            BleDeviceError error) {
                                                auto neuroDevice = neuroDevicePtr.lock();
                                                if (neuroDevice) {
                                                    neuroDevice->onBleDeviceStateChanged(state, error);
                                                }
                                        });
    bleDevice->deviceStateChanged += bleDeviceStateHandler;
}

void Device::subscribeDataReceived() {
    std::weak_ptr<Device> neuroDevicePtr = shared_from_this();
    dataReceivedHandler = MakeHandler(BleDevice, dataReceived,
                                      [neuroDevicePtr](const BleDevice &, const std::vector<Byte> &data) {
                                          auto neuroDevice = neuroDevicePtr.lock();
                                          if (neuroDevice) {
                                              neuroDevice->onDataReceived(data);
                                          }
                                      });
    bleDevice->dataReceived += dataReceivedHandler;
}

void Device::subscribeStatusReceived() {
    std::weak_ptr<Device> neuroDevicePtr = shared_from_this();
    statusReceivedHandler = MakeHandler(BleDevice, statusReceived,
                                        [neuroDevicePtr](const BleDevice &, const std::vector<Byte> &status) {
                                                auto neuroDevice = neuroDevicePtr.lock();
                                                if (neuroDevice) {
                                                    neuroDevice->onStatusDataReceived(status);
                                                }
                                        });
    bleDevice->statusReceived += statusReceivedHandler;
}

*/





















