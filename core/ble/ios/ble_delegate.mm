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

#import "ble/ios/ble_delegate.h"
#include <list>


@implementation CBScannerDelegate
{
    void (^deviceFoundCallbackFunc)(CBPeripheral*);
    std::list<std::pair<CBPeripheral*, void(^)()>> deviceConnectedCallbacks;
    std::list<std::pair<CBPeripheral*, void(^)()>> deviceDisconnectedCallbacks;
    std::list<std::pair<CBPeripheral*, void(^)()>> deviceConnectionErrorCallbacks;
}

-(void) setDeviceFoundCallback: (void (^)(CBPeripheral*))deviceFoundCallback
{
    deviceFoundCallbackFunc = deviceFoundCallback;
}

-(void) setDeviceConnectedCallback: (CBPeripheral*)device callback:(void(^)())callback
{
    NSLog(@"Setting connected callback");
    deviceConnectedCallbacks.push_back(std::pair<CBPeripheral*, void(^)()>(device, callback));
}

-(void) setDeviceDisconnectedCallback: (CBPeripheral*)device callback:(void(^)())callback
{
    deviceDisconnectedCallbacks.push_back(std::pair<CBPeripheral*, void(^)()>(device, callback));
}

-(void) setDeviceConnectionErrorCallback: (CBPeripheral*)device callback:(void(^)())callback
{
    deviceConnectionErrorCallbacks.push_back(std::pair<CBPeripheral*, void(^)()>(device, callback));
}


- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    
}

-(void) removeDeviceConnectedCallback: (CBPeripheral*)device
{
    NSLog(@"Remove device connected callback");
    if (!deviceConnectedCallbacks.size()) return;
    
    for (auto it = deviceConnectedCallbacks.begin(); it!=deviceConnectedCallbacks.end(); ++it)
    {
        if (it->first == device)
        {
            deviceConnectedCallbacks.remove(*it);
        }
    }
}

-(void) removeDeviceDisconnectedCallback: (CBPeripheral*)device
{
    if (!deviceDisconnectedCallbacks.size()) return;
    
    for (auto it = deviceDisconnectedCallbacks.begin(); it!=deviceDisconnectedCallbacks.end(); ++it)
    {
        if (it->first == device)
        {
            deviceDisconnectedCallbacks.remove(*it);
        }
    }
}

-(void) removeDeviceConnectionErrorCallback: (CBPeripheral*)device
{
    if (!deviceConnectionErrorCallbacks.size()) return;
    
    for (auto it = deviceConnectionErrorCallbacks.begin(); it!=deviceConnectionErrorCallbacks.end(); ++it)
    {
        if (it->first == device)
        {
            deviceConnectionErrorCallbacks.remove(*it);
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI
{
        if (deviceFoundCallbackFunc) deviceFoundCallbackFunc(peripheral);
    
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    NSLog(@"Connected to peripheral");
    if (!deviceConnectedCallbacks.size()) return;
    
    NSLog(@"Notifiyng all");
    for (auto it = deviceConnectedCallbacks.begin(); it!=deviceConnectedCallbacks.end(); ++it)
    {
        if (it->first == peripheral)
        {
            it->second();
            break;
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    if (!deviceConnectionErrorCallbacks.size()) return;
    
    for (auto it = deviceConnectionErrorCallbacks.begin(); it!=deviceConnectionErrorCallbacks.end(); ++it)
    {
        if (it->first == peripheral)
        {
            it->second();
            break;
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    if (!deviceDisconnectedCallbacks.size()) return;
    
    for (auto it = deviceDisconnectedCallbacks.begin(); it!=deviceDisconnectedCallbacks.end(); ++it)
    {
        if (it->first == peripheral)
        {
            it->second();
            break;
        }
    }
}

@end


@implementation CBDeviceDelegate
{
    std::shared_ptr<DeviceGattInfo> deviceGattInfo;
    std::shared_ptr<DeviceCommInterface> deviceCommCallback;
    CBCharacteristic* txCharacteristic;
    CBPeripheral* transmitPeripheral;
    CBCharacteristic* rxCharacteristic;
    CBCharacteristic* statusCharacteristic;
}

-(id)initWithGattInfo:(std::shared_ptr<DeviceGattInfo>)gattInfo
{
    if (self = [super init])
    {
        deviceGattInfo = gattInfo;
    }
    return self;
}


-(bool)sendMessage:(void*)data length:(size_t)length
{
    if (data == nullptr || length == 0) return false;
    NSLog(@"Data ptr: %p, length is %zd", data, length);
    auto nsData = [NSData dataWithBytes:data length:length];

    if (txCharacteristic == nil || transmitPeripheral == nil) return false;
    
    [transmitPeripheral writeValue:nsData forCharacteristic:txCharacteristic type:CBCharacteristicWriteWithoutResponse];
    return true;
}


-(void)setCommunicationCallbacks: (std::shared_ptr<DeviceCommInterface>) commInterface
{
    NSLog(@"Set device comm callback");
    deviceCommCallback = commInterface;
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(nullable NSError *)error
{
    if (error) {
        if (deviceCommCallback)
            deviceCommCallback->onBleDeviceError();
        return;
    }
    
    for (CBService* service in peripheral.services)
    {
        [peripheral discoverCharacteristics:nil forService:service];
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(nullable NSError *)error
{
    if (error) {
        if (deviceCommCallback)
            deviceCommCallback->onBleDeviceError();
        return;
    }
    
    for (CBCharacteristic* characteristic in service.characteristics)
    {
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[NSString stringWithCString:deviceGattInfo->rxCharacteristicUUID().c_str() encoding:NSASCIIStringEncoding]]])
        {
            NSLog(@"RX characteristic found");
            rxCharacteristic = characteristic;
            [peripheral setNotifyValue:YES forCharacteristic:characteristic];
            deviceCommCallback->onReceiveReady();
        }
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[NSString stringWithCString:deviceGattInfo->txCharacteristicUUID().c_str() encoding:NSASCIIStringEncoding]]])
        {
            NSLog(@"TX characteristic found");
            txCharacteristic = characteristic;
            transmitPeripheral = peripheral;
            deviceCommCallback->onTransmitReady();
        }
        
        if (!deviceGattInfo->statusCharacteristicUUID().empty())
        {
        if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[NSString stringWithCString:deviceGattInfo->statusCharacteristicUUID().c_str() encoding:NSASCIIStringEncoding]]])
        {
            NSLog(@"Status characteristic found");
            statusCharacteristic = characteristic;
            [peripheral setNotifyValue:YES forCharacteristic:characteristic];
            deviceCommCallback->onStatusReceiveReady();
        }
        }
    }
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(nullable NSError *)error
{
    if (error) {
        NSLog(@"Receive error");
        if (deviceCommCallback)
           deviceCommCallback->onBleDeviceError();
        return;
    }
    NSLog(@"Data received");
    if (characteristic == rxCharacteristic)
    {
        deviceCommCallback->onDataReceived([characteristic.value bytes], [characteristic.value length]);
        return;
    }
    
    if (characteristic == statusCharacteristic)
    {
        deviceCommCallback->onStatusReceived([characteristic.value bytes], [characteristic.value length]);
    }
}


@end
