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

#ifndef ANDROID_BLE_DEVICE_JNI_H
#define ANDROID_BLE_DEVICE_JNI_H

#include "device/ble_device.h"
#include "java_environment.h"

class BleDeviceJni : public BleDevice {
public:
    BleDeviceJni(jobject bluetoothDevice, jobject context);

    BleDeviceJni(const BleDeviceJni &) = delete;

    BleDeviceJni &operator=(const BleDeviceJni &) = delete;

    ~BleDeviceJni();

    void connect() override;

    void disconnect() override;

    void close() override;

    bool sendCommand(const std::vector<Byte> &commandData) override;

    std::string getName() const override;

    std::string getNetAddress() const override;

    BleDeviceState getState() const override;

    void onConnected();
    void onDisconnected();
    void onError(BleDeviceError);
    void onDataReceived(const std::vector<Byte> &);
    void onStatusReceived(const std::vector<Byte> &);

private:
    jobject javaBleDevice;
    jobject appContext;
};

#endif //ANDROID_BLE_DEVICE_JNI_H
