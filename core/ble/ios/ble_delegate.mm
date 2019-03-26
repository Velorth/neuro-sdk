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


@implementation ScannerDelegate
{
    void (^mDeviceFoundCallbackFunc)(CBPeripheral*, NSNumber *rssi);
}

-(id) initWithDeviceFoundCallback:(void (^)(CBPeripheral*, NSNumber*))deviceFoundCallback
{
    if (self = [super init])
    {
        mDeviceFoundCallbackFunc = deviceFoundCallback;
    }
    return self;
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI
{
    if (mDeviceFoundCallbackFunc) mDeviceFoundCallbackFunc(peripheral, RSSI);
}

@end


@implementation ConnectionDelegate
{
    void(^mDeviceConnectedCallback)(CBPeripheral *);
    void(^mDeviceDisconnectedCallback)(CBPeripheral *);
    void(^mDeviceConnectionErrorCallback)(CBPeripheral *);
}

-(id) initWithConnectionCallbacks:(void(^)(CBPeripheral *))connectedCallback disconnectedCallback:(void(^)(CBPeripheral *))disconnectedCallback errorCallback:(void(^)(CBPeripheral *))errorCallback;
{
    if (self = [super init])
    {
        mDeviceConnectedCallback = connectedCallback;
        mDeviceDisconnectedCallback = disconnectedCallback;
        mDeviceConnectionErrorCallback = errorCallback;
    }
    return self;
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral
{
    if (mDeviceConnectedCallback){
        mDeviceConnectedCallback(peripheral);
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    if (mDeviceConnectionErrorCallback){
        mDeviceConnectionErrorCallback(peripheral);
    }
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error
{
    if (mDeviceDisconnectedCallback){
        mDeviceDisconnectedCallback(peripheral);
    }
}
@end


@implementation DeviceDelegate
{
    void(^mServicesDiscoveredCallback)();
    void(^mCharacteristicsDiscoveredCallback)(CBService *);
    void(^mCharacteristicChangedCallback)(CBCharacteristic *);
}

-(id)initWithCallbacks:(void(^)())servicesDiscoveredCallback
    characteristicsDiscoveredCallback:(void(^)(CBService *))characteristicsDiscoveredCallback
    characteristicChangedCallback:(void(^)(CBCharacteristic *))characteristicChangedCallback
{
    if (self = [super init])
    {
        mServicesDiscoveredCallback = servicesDiscoveredCallback;
        mCharacteristicsDiscoveredCallback = characteristicsDiscoveredCallback;
        mCharacteristicChangedCallback = characteristicChangedCallback;
    }
    return self;
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(nullable NSError *)error
{
    if (error) return;
    if (mServicesDiscoveredCallback) mServicesDiscoveredCallback();
}

- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(nullable NSError *)error
{
    if (error) return;
    if (mCharacteristicsDiscoveredCallback) mCharacteristicsDiscoveredCallback(service);
}

- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(nullable NSError *)error
{
    if (error) return;
    if (mCharacteristicChangedCallback) mCharacteristicChangedCallback(characteristic);
}


@end
