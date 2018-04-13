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

#ifndef ble_scanner_objc_h
#define ble_scanner_objc_h

#import "ble_delegate.h"
#include "device_scanner.h"


class NCBleScanner: public DeviceScanner
{
public:
    NCBleScanner();
     ~NCBleScanner();
     void startScan();
     void stopScan();
     std::shared_ptr<BleDevice> getDeviceByAddress(std::string);
     void setFilter(std::vector<std::string>);
     void subscribeDeviceFound(std::function<void(std::shared_ptr<BleDevice>)>);
     std::vector<std::shared_ptr<BleDevice>> getDevices();
     bool isScanning();
private:
    CBCentralManager* scanner;
    CBScannerDelegate* scanDlg;
    
    std::vector<CBPeripheral*> foundDevices;
    std::vector<std::string> filters;
    
    std::function<void(std::shared_ptr<BleDevice>)> deviceFoundCallbackFunc;
};


#endif /* ble_scanner__objc_h */
