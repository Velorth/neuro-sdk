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

#ifndef ble_delegate_h
#define ble_delegate_h

#import <CoreBluetooth/CoreBluetooth.h>
#include "device_info.h"
#include "device_comm_interface.h"

@interface CBScannerDelegate:NSObject <CBCentralManagerDelegate>

-(void) setDeviceFoundCallback: (void (^)(CBPeripheral*))deviceFoundCallback;

-(void) setDeviceConnectedCallback: (CBPeripheral*)device callback:(void(^)())callback;
-(void) setDeviceDisconnectedCallback: (CBPeripheral*)device callback:(void(^)())callback;
-(void) setDeviceConnectionErrorCallback: (CBPeripheral*)device callback:(void(^)())callback;

-(void) removeDeviceConnectedCallback: (CBPeripheral*)device;
-(void) removeDeviceDisconnectedCallback: (CBPeripheral*)device;
-(void) removeDeviceConnectionErrorCallback: (CBPeripheral*)device;
@end

@interface CBDeviceDelegate:NSObject <CBPeripheralDelegate>

-(id)initWithGattInfo: (std::shared_ptr<DeviceGattInfo>) gattInfo;

-(bool)sendMessage:(void*)data length:(size_t)length;
-(void)setCommunicationCallbacks: (std::shared_ptr<DeviceCommInterface>) commInterface;

@end

#endif /* ble_delegate_h */
