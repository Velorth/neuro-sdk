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
#include "ble/ios/ble_delegate.h"
#include "device/device_info.h"

namespace Neuro {

CBPeripheral* peripheral_from_id(CBCentralManager *central_manger, NSUUID *identifier, DeviceDelegate *device_delegate){
    auto peripherals = [central_manger retrievePeripheralsWithIdentifiers:@[identifier]];
    if ([peripherals count] > 0){
        auto device = [peripherals firstObject];
        device.delegate = device_delegate;
        return device;
    }
    throw std::runtime_error("Device is unreachable");
}

struct NCBleDevice::Impl {
    ConnectionDelegate* mConnectionDelegate;
    CBCentralManager* mScanner;
    DeviceDelegate* mDeviceDelegate;
    CBPeripheral* mPeripheral;
    CBCharacteristic* mTxCharacteristic;
    CBCharacteristic* mRxCharacteristic;
    CBCharacteristic* mStatusCharacteristic;
    
    Impl(ConnectionDelegate *connection_delegate, DeviceDelegate *device_delegate, NSUUID *identifier):
        mConnectionDelegate(connection_delegate),
        mScanner([[CBCentralManager alloc] initWithDelegate:mConnectionDelegate queue:dispatch_queue_create("central_queue", 0)]),
        mDeviceDelegate(device_delegate),
        mPeripheral(peripheral_from_id(mScanner, identifier, mDeviceDelegate)) {
    
    }

    void onPeripheralConnected(CBPeripheral *peripheral){
        if (peripheral == mPeripheral){
            [mPeripheral discoverServices:nil];
        }
    }
    
    void onServicesDiscovered(){
        for (CBService *service in mPeripheral.services){
            [mPeripheral discoverCharacteristics:nil forService:service];
        }
    }
    
    void onCharacteristicsDiscovered(CBService *service, std::shared_ptr<DeviceGattInfo> deviceGattInfo){
        for (CBCharacteristic* characteristic in service.characteristics)
        {
            if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[NSString stringWithCString:deviceGattInfo->rxCharacteristicUUID().c_str() encoding:NSASCIIStringEncoding]]])
            {
                mRxCharacteristic = characteristic;
                [mPeripheral setNotifyValue:YES forCharacteristic:characteristic];
            }
            if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[NSString stringWithCString:deviceGattInfo->txCharacteristicUUID().c_str() encoding:NSASCIIStringEncoding]]])
            {
                mTxCharacteristic = characteristic;
            }
            if (!deviceGattInfo->statusCharacteristicUUID().empty())
            {
                if ([characteristic.UUID isEqual:[CBUUID UUIDWithString:[NSString stringWithCString:deviceGattInfo->statusCharacteristicUUID().c_str() encoding:NSASCIIStringEncoding]]]){
                    mStatusCharacteristic = characteristic;
                    [mPeripheral setNotifyValue:YES forCharacteristic:characteristic];
                }
            }
        }
    }
};

NSUUID *identifier_from_string(const std::string &id_string){
    return [[NSUUID alloc] initWithUUIDString: [NSString stringWithUTF8String:id_string.c_str()]];
}

NCBleDevice::NCBleDevice(const DeviceInfo &device_info):
    BleDevice(BleDeviceInfo::fromDeviceName(device_info.Name)),
    mImpl(std::make_unique<Impl>(
        [[ConnectionDelegate alloc] initWithConnectionCallbacks:
            ^(CBPeripheral *peripheral){ mImpl->onPeripheralConnected(peripheral); }
            disconnectedCallback:^(CBPeripheral *peripheral){ notifyStateChanged(BleDeviceState::Disconnected, BleDeviceError::NoError); }
            errorCallback:^(CBPeripheral *peripheral){ notifyStateChanged(BleDeviceState::Disconnected, BleDeviceError::GeneralConnectionError); }
        ],
        [[DeviceDelegate alloc] initWithCallbacks:^(){mImpl->onServicesDiscovered();}
            characteristicsDiscoveredCallback:^(CBService *service){
                mImpl->onCharacteristicsDiscovered(service, getGattInfo()->getGattInfo());
                notifyStateChanged(BleDeviceState::Connected, BleDeviceError::NoError);
            }
            characteristicChangedCallback:^(CBCharacteristic *characteristic){
                auto first = static_cast<const Byte *>([characteristic.value bytes]);
                auto last = first + [characteristic.value length];
                std::vector<Byte> data(first, last);
                if (characteristic == mImpl->mStatusCharacteristic){
                    notifyStatusReceived(data);
                }
                else if(characteristic == mImpl->mRxCharacteristic){
                    notifyDataReceived(data);
                }
            }
        ],
        identifier_from_string(device_info.Address)
    )){

}

NCBleDevice::~NCBleDevice() = default;

void NCBleDevice::connect() {
    [mImpl->mScanner connectPeripheral:mImpl->mPeripheral options:nil];
}

void NCBleDevice::disconnect() {
    [mImpl->mScanner cancelPeripheralConnection:mImpl->mPeripheral];
}

void NCBleDevice::close() {
    [mImpl->mScanner cancelPeripheralConnection:mImpl->mPeripheral];
}

bool NCBleDevice::sendCommand(const std::vector<Byte> &commandData){
    if (mImpl->mTxCharacteristic == nil || mImpl->mPeripheral == nil)
        return false;
    
    auto nsData = [NSData dataWithBytes:commandData.data() length:commandData.size()];
    [mImpl->mPeripheral writeValue:nsData forCharacteristic:mImpl->mTxCharacteristic type:CBCharacteristicWriteWithoutResponse];
    return true;
}

std::string NCBleDevice::getName() const {
    return std::string([[mImpl->mPeripheral name] cStringUsingEncoding:NSASCIIStringEncoding]);
}

std::string NCBleDevice::getNetAddress() const {
    return std::string([[[mImpl->mPeripheral identifier] UUIDString] cStringUsingEncoding:NSASCIIStringEncoding]);
}

BleDeviceState NCBleDevice::getState() const {
    if (mImpl->mPeripheral.state == CBPeripheralStateConnected){
        return BleDeviceState::Connected;
    }
    else {
        return BleDeviceState::Disconnected;
    }
}
    
}
