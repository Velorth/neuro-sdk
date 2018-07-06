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

#include "neurosdk.h"
#include "method_connection.h"
#include "stephelper_device.h"
#include "ble_scanner_objc.h"
#include "logger_objc.h"

@implementation NCMotionAssistantConnection
{
    MethodDeviceConnection<StepHelperDevice>* motionAssistantConnection;
}

-(id)init
{
    if (self = [super init])
    {
        auto logFactory = LoggerFactory::getInstance();
        logFactory->setLogger(new ObjcLogger());
        logFactory->setLogLevel(LogLevel:: Info); // Trace
        auto bleScanner = std::make_shared<NCBleScanner>();
        auto neuroConnection = std::make_shared<NeuroConnection>(bleScanner);
        motionAssistantConnection = new MethodDeviceConnection<StepHelperDevice>(neuroConnection);
    }
    
    return self;
}

-(void)dealloc
{
    delete motionAssistantConnection;
}

-(void)startScan:(int)timeout
{
    motionAssistantConnection->startScan(timeout);
}

-(void)stopScan
{
    motionAssistantConnection->stopScan();
}

-(NCMotionAssistantDevice *)getDeviceByAddress:(const NSString*)address
{
    auto device = motionAssistantConnection->getDeviceByAddress([address cStringUsingEncoding:NSASCIIStringEncoding]);
    return [[NCMotionAssistantDevice alloc] initWithNativeDevice:new std::shared_ptr<StepHelperDevice>(device)];
}

-(void)setScanStateChangedCallback:(void(^)(bool)) scanStateChangedCallback
{
    motionAssistantConnection->subscribeScanStateChanged([scanStateChangedCallback](bool isScanning){
        scanStateChangedCallback(isScanning);
    });
}

-(void)setDeviceFoundCallback:(void(^)(NCMotionAssistantDevice *)) deviceFoundCallback
{
    motionAssistantConnection->subscribeDeviceFound([deviceFoundCallback](std::shared_ptr<StepHelperDevice> foundDevice){
        auto device = [[NCMotionAssistantDevice alloc] initWithNativeDevice:new std::shared_ptr<StepHelperDevice>(foundDevice)];
        deviceFoundCallback(device);
    });
}

@end
