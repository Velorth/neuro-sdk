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

#include "ble_scanner_objc.h"
#include "ble_device_objc.h"

using std::shared_ptr;
using std::vector;
using std::string;

NCBleScanner::NCBleScanner():
    scanner(nullptr),
    scanDlg([[CBScannerDelegate alloc] init])
{
    scanner = [[CBCentralManager alloc] initWithDelegate:scanDlg queue:dispatch_queue_create("central_queue", 0)];
}

NCBleScanner::~NCBleScanner()
{
    
}

void NCBleScanner::startScan()
{
    NSLog(@"Start scan");
    foundDevices.clear();
    [scanner scanForPeripheralsWithServices:nil options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @FALSE}];
}

void NCBleScanner::stopScan()
{
    NSLog(@"Stop scan");
    [scanner stopScan];
}

shared_ptr<BleDevice> NCBleScanner::getDeviceByAddress(string address)
{
    auto nsAddress = [[NSString alloc] initWithCString:address.c_str() encoding:NSASCIIStringEncoding];
    auto bleDevices = [scanner retrievePeripheralsWithIdentifiers:@[[[NSUUID alloc] initWithUUIDString:nsAddress]]];
    
    //if (![bleDevices count]) return std::make_shared<NCBleDevice>(nullptr);
    
    return std::make_shared<NCBleDevice>(bleDevices[0], scanner, scanDlg);
}

void NCBleScanner::setFilter(vector<string> deviceNames)
{
    filters = deviceNames;
    NSLog(@"Setting filters:");
    for (auto it = filters.begin(); it!=filters.end(); ++it)
    {
        NSLog(@"Filter: %@", [NSString stringWithCString:it->c_str() encoding:NSASCIIStringEncoding]);
    }
}

void NCBleScanner::subscribeDeviceFound(std::function<void(shared_ptr<BleDevice>)> callback)
{
    NSLog(@"Subscribing device found");
    deviceFoundCallbackFunc = callback;
    [scanDlg setDeviceFoundCallback: ^(CBPeripheral* device)
     {
         if (device == nullptr || device.name == nullptr){
             return;
         }
             
         NSLog(@"Device %@ been found", device.name);
         auto name = std::string([device.name cStringUsingEncoding:NSASCIIStringEncoding]);
         if (std::find(filters.begin(), filters.end(), name) == filters.end()) return;
         
         if (std::find(foundDevices.begin(), foundDevices.end(), device)!=foundDevices.end()) return;
         foundDevices.push_back(device);
         
         NSLog(@"Notifiyng device found");
        deviceFoundCallbackFunc(std::make_shared<NCBleDevice>(device, scanner, scanDlg));
     }];
}

vector<shared_ptr<BleDevice>> NCBleScanner::getDevices()
{
    return vector<shared_ptr<BleDevice>>();
}

bool NCBleScanner::isScanning()
{
    return [scanner isScanning];
}

