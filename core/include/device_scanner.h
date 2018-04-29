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

#ifndef NEURO_CONNECTION_H
#define NEURO_CONNECTION_H

#include <mutex>
#include <condition_variable>
#include "device/device.h"
#ifdef __ANDROID__
#include <jni.h>
#endif

namespace Neuro {

class BleScanner;
class BleDevice;

class DeviceScanner
{
public:
    DeviceScanner(std::unique_ptr<BleScanner> deviceScanner);
    DeviceScanner(const DeviceScanner&) = delete;
    DeviceScanner& operator=(const DeviceScanner&) = delete;
    ~DeviceScanner();

    void startScan(int timeout);
    void stopScan();
    void subscribeDeviceFound(std::function<void(std::unique_ptr<Device>)>);
    void subscribeScanStateChanged(std::function<void(bool)>);
    std::unique_ptr<Device> findDeviceByAddress(std::string address);
    void releaseDevice(std::string name, std::string address);
private:

    std::shared_ptr<BleScanner> scanner;
    std::condition_variable stopScanCondition;
    std::mutex stopScanMutex;
    std::function<void(std::unique_ptr<Device>)> deviceFoundCallback;
    std::function<void(bool)> scanStateChangedCallback;

    std::unique_ptr<Device> onNewBleDevice(std::unique_ptr<BleDevice>);
};

#ifdef __ANDROID__
std::unique_ptr<DeviceScanner> createDeviceScanner(jobject context);
#else
std::unique_ptr<DeviceScanner> createDeviceScanner();
#endif

}
#endif //NEURO_CONNECTION_H
