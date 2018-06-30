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

#ifndef DEVICE_SCANNER_H
#define DEVICE_SCANNER_H

#include <string>
#include <vector>
#include <functional>
#include "ble/ble_device.h"
#include "ble/emulator/emulator_client.h"

namespace Neuro {

class BleScanner
{
public:
    virtual ~BleScanner() = default;
    virtual void startScan() = 0;
    virtual void stopScan() = 0;
    virtual std::unique_ptr<BleDevice> getDeviceByAddress(std::string) = 0;
    virtual void setFilter(std::vector<std::string>) = 0;
    virtual void subscribeDeviceFound(std::function<void(std::unique_ptr<BleDevice>)>) = 0;
    virtual bool isScanning() = 0;
    virtual void releaseDevice(std::string name, std::string address) = 0;

protected:
    EmulatorClient emulator;
};

}

#endif //DEVICE_SCANNER_H
