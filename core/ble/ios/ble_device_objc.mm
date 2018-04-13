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

#include "ble_device_objc.h"

NCBleDevice::NCBleDevice(CBPeripheral* device, CBCentralManager* central, CBScannerDelegate* scanDelegate):
    hardwareDevice(device),
    scanner(central),
    scannerDelegate(scanDelegate)
{
    NSLog(@"Setting ble callbacks");
    [scannerDelegate setDeviceConnectedCallback:device callback:^{onDeviceConnected();}];
    [scannerDelegate setDeviceDisconnectedCallback:device callback:^{onDeviceDisconnected();}];
    [scannerDelegate setDeviceConnectionErrorCallback:device callback:^{onDeviceError();}];
    
    deviceInfo = DeviceInfo::fromDeviceName([[device name]cStringUsingEncoding:NSASCIIStringEncoding]);
    deviceDelegate = [[CBDeviceDelegate alloc] initWithGattInfo:deviceInfo->getGattInfo()];
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

bool NCBleDevice::sendCommand(void* command_data, size_t length)
{
    return [deviceDelegate sendMessage: command_data length:length];
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

void NCBleDevice::setDeviceCallback(std::shared_ptr<DeviceCommInterface> callbackInterface)
{
    NSLog(@"Set callback");
    deviceCallback = callbackInterface;
    [deviceDelegate setCommunicationCallbacks:callbackInterface];
}

void NCBleDevice::onDeviceConnected()
{
    NSLog(@"Device connected");
    hardwareDevice.delegate = deviceDelegate;
    deviceCallback->onConnected();
    [hardwareDevice discoverServices:nil];
}

void NCBleDevice::onDeviceDisconnected()
{
    NSLog(@"Device disconnected");
    deviceCallback->onDisconnected();
}

void NCBleDevice::onDeviceError()
{
    deviceCallback->onBleDeviceError();
}
