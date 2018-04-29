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

#ifndef ble_device_objc_h
#define ble_device_objc_h

#include "ble_device.h"
#import "ble_delegate.h"

class NCBleDevice: public BleDevice
{
public:
    NCBleDevice(CBPeripheral*, CBCentralManager*, CBScannerDelegate*);
    NCBleDevice(const NCBleDevice&) = delete;
    NCBleDevice& operator=(const NCBleDevice&) = delete;
    ~NCBleDevice();
    
    void connect();
    void disconnect();
    void close();
    bool sendCommand(void*, size_t);
    std::string getName() const;
    std::string getNetAddress() const;
    void setDeviceCallback(std::shared_ptr<DeviceCommInterface> callbackInterface);
private:
    CBPeripheral* hardwareDevice;
    CBCentralManager* scanner;
    CBDeviceDelegate* deviceDelegate;
    CBScannerDelegate* scannerDelegate;
    std::shared_ptr<DeviceCommInterface> deviceCallback;
    
    void onDeviceConnected();
    void onDeviceDisconnected();
    void onDeviceError();
};

#endif /* ble_device_objc_h */
