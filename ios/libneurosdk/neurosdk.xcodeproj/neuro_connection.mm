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

#import "neurosdk.h"
#include "neuro_connection.h"
#include "ble_scanner_objc.h"
#include "logger_objc.h"

@implementation NCNeuroConnection
{
    NeuroConnection* neuroConnection;
}

-(id)init
{
    if (self = [super init])
    {
        auto logFactory = LoggerFactory::getInstance();
        logFactory->setLogger(new ObjcLogger());
        logFactory->setLogLevel(LogLevel::Trace);
        auto bleScanner = std::make_shared<NCBleScanner>();
        neuroConnection = new NeuroConnection(bleScanner);
    }
    
    return self;
}

-(void)dealloc
{
    delete neuroConnection;
}

-(void)startScan:(int)timeout
{
    neuroConnection->startScan(timeout);
}

-(void)stopScan
{
    neuroConnection->stopScan();
}

-(void)subscribeDeviceFound:(void (^)(NCNeuroDevice*))deviceFoundCallback
{
    neuroConnection->subscribeDeviceFound([deviceFoundCallback](std::shared_ptr<NeuroDevice> neuroDevice)
    {
        auto ncDevice = [[NCNeuroDevice alloc] initWithNeuroDevice: new std::shared_ptr<NeuroDevice>(neuroDevice)];
        deviceFoundCallback(ncDevice);
    });
}

-(void)subscribeScanStateChanged: (void (^)(bool)) scanStateChangedCallback
{
    neuroConnection->subscribeScanStateChanged([scanStateChangedCallback](bool state){scanStateChangedCallback(state);});
}

-(NCNeuroDevice*)findDeviceByAddress:(const NSString*)address
{
    auto neuroDevice = neuroConnection->findDeviceByAddress([address cStringUsingEncoding:NSASCIIStringEncoding]);
    return [[NCNeuroDevice alloc] initWithNeuroDevice:new std::shared_ptr<NeuroDevice>(neuroDevice)];
}

@end
