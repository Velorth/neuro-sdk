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

#include "ble/ios/ble_device_objc.h"

namespace Neuro {
    
NCBleDevice::NCBleDevice(CBPeripheral* device, CBCentralManager* central, CBScannerDelegate* scanDelegate):
    BleDevice(BleDeviceInfo::fromDeviceName([[device name]cStringUsingEncoding:NSASCIIStringEncoding])),
    hardwareDevice(device),
    scanner(central),
    scannerDelegate(scanDelegate)
{
    NSLog(@"Setting ble callbacks");
    [scannerDelegate setDeviceConnectedCallback:device callback:^{onDeviceConnected();}];
    [scannerDelegate setDeviceDisconnectedCallback:device callback:^{onDeviceDisconnected();}];
    [scannerDelegate setDeviceConnectionErrorCallback:device callback:^{onDeviceError();}];
    
    deviceDelegate = [[CBDeviceDelegate alloc] initWithGattInfo:mDeviceInfo->getGattInfo()];
}

NCBleDevice::~NCBleDevice()
{
    NSLog(@"BleDevice destructor");
    [scannerDelegate removeDeviceConnectedCallback:hardwareDevice];
    [scannerDelegate removeDeviceDisconnectedCallback:hardwareDevice];
    [scannerDelegate removeDeviceConnectionErrorCallback:hardwareDevice];
}

void NCBleDevice::connect()
{
    NSLog(@"Connect");
    [scanner connectPeripheral:hardwareDevice options:nil];
}

void NCBleDevice::disconnect()
{
    NSLog(@"Disconnect");
    [scanner cancelPeripheralConnection:hardwareDevice];
}

void NCBleDevice::close()
{
    [scanner cancelPeripheralConnection:hardwareDevice];
}

bool NCBleDevice::sendCommand(const std::vector<Byte> &commandData)
{
    auto bufferCopy = commandData;
    return [deviceDelegate sendMessage: bufferCopy.data() length:bufferCopy.size()];
}

std::string NCBleDevice::getName() const
{
    NSLog(@"Get name");
    return std::string([[hardwareDevice name] cStringUsingEncoding:NSASCIIStringEncoding]);
}

std::string NCBleDevice::getNetAddress() const
{
    NSLog(@"Get address");
    return std::string([[[hardwareDevice identifier] UUIDString] cStringUsingEncoding:NSASCIIStringEncoding]);
}

void NCBleDevice::onDeviceConnected()
{
    NSLog(@"Device connected");
    hardwareDevice.delegate = deviceDelegate;
    deviceStateChangedCallback(BleDeviceState::Connected, BleDeviceError::NoError);
    [hardwareDevice discoverServices:nil];
}

void NCBleDevice::onDeviceDisconnected()
{
    NSLog(@"Device disconnected");
    deviceStateChangedCallback(BleDeviceState::Disconnected, BleDeviceError::NoError);
}

void NCBleDevice::onDeviceError()
{
    deviceStateChangedCallback(BleDeviceState::Disconnected, BleDeviceError::GeneralConnectionError);
}
    
}
