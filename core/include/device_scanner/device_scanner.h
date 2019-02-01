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

#include "device/device.h"
#include "lib_export.h"

namespace Neuro {

class BleScanner;
class BleDevice;

class SDK_SHARED DeviceScanner final
{
public:
    DeviceScanner(std::unique_ptr<BleScanner> device_scanner);
    DeviceScanner(const DeviceScanner&) = delete;
    DeviceScanner& operator=(const DeviceScanner&) = delete;
    ~DeviceScanner();

    void startScan(int timeout_ms);
    void stopScan();
	ListenerPtr<void, DeviceSharedPtr> subscribeDeviceFound(std::function<void(DeviceSharedPtr)>);
	ListenerPtr<void, bool> subscribeScanStateChanged(std::function<void(bool)>);
    DeviceUniquePtr findDeviceByAddress(std::string address);
	bool isScanning() const noexcept;

private:
	class Impl;
	std::unique_ptr<Impl> mImpl;
};

}
#endif //NEURO_CONNECTION_H
