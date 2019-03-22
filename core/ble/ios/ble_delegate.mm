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
    void (^mDeviceFoundCallbackFunc)(CBPeripheral*);
    std::unordered_map<CBPeripheral*, void(^)()> mDeviceConnectedCallbacks;
    std::unordered_map<CBPeripheral*, void(^)()> mDeviceDisconnectedCallbacks;
    std::unordered_map<CBPeripheral*, void(^)()> mDeviceConnectionErrorCallbacks;
}

-(id) initWithDeviceFoundCallback:(void (^)(CBPeripheral*))deviceFoundCallback
{
    if (self = [super init])
    {
        mDeviceFoundCallbackFunc = deviceFoundCallback;
    }
    return self;
}

-(void) setDeviceConnectedCallback: (CBPeripheral*)device callback:(void(^)())callback
{
    mDeviceConnectedCallbacks[device] = callback;
}

-(void) setDeviceDisconnectedCallback: (CBPeripheral*)device callback:(void(^)())callback
{
    mDeviceDisconnectedCallbacks[device] = callback;
}

-(void) setDeviceConnectionErrorCallback: (CBPeripheral*)device callback:(void(^)())callback
{
    mDeviceConnectionErrorCallbacks[device] = callback;
}


- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    
}

-(void) removeDeviceConnectedCallback: (CBPeripheral*)device
{
    mDeviceConnectedCallbacks.erase(device);
}

-(void) removeDeviceDisconnectedCallback: (CBPeripheral*)device
{
    mDeviceDisconnectedCallbacks.erase(device);
}

-(void) removeDeviceConnectionErrorCallback: (CBPeripheral*)device
{
    mDeviceConnectionErrorCallbacks.erase(device);
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI
{
    if (mDeviceFoundCallbackFunc) mDeviceFoundCallbackFunc(peripheral);
    
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    if (mDeviceConnectedCallbacks.find(device) != mDeviceConnectedCallbacks.end()){
        mDeviceConnectedCallbacs[device]()
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    if (mDeviceConnectionErrorCallbacks.find(device) != mDeviceConnectionErrorCallbacks.end()){
        mDeviceConnectionErrorCallbacks[device]()
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    if (mDeviceDisconnectedCallbacks.find(device) != mDeviceDisconnectedCallbacks.end()){
        mDeviceDisconnectedCallbacks[device]()
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
