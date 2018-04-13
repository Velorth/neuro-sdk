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
#include "neuro_device_wrap.h"
#include "neuro_device.h"
#include "logger.h"

@implementation NCNeuroDevice
{
    void *neuroDevice;
}

-(id)initWithNeuroDevice: (void*)device
{
    if (self = [super init]){
        neuroDevice = device;
    }
    return self;
}

-(void)dealloc
{
    if (neuroDevice!=nullptr)
    {
        auto device = (std::shared_ptr<NeuroDevice> *)neuroDevice;
        delete device;
        neuroDevice = nullptr;
    }
}
-(void)close
{
    if (neuroDevice!=nullptr)
    {
        auto device = (std::shared_ptr<NeuroDevice> *)neuroDevice;
        (*device)->disconnect();
        delete device;
        neuroDevice = nullptr;
    }
}

-(void)connect
{
    neuroDeviceConnect(neuroDevice);
}
-(void)disconnect
{
    neuroDeviceDisconnect(neuroDevice);
}

-(const NSString*)getName
{
    char nameBuffer[255];
    neuroDeviceGetName(neuroDevice, nameBuffer, sizeof(nameBuffer)/sizeof(nameBuffer[0]));
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s] before nsstring name: %s", __FUNCTION__, nameBuffer);
    auto nsDevName = [NSString stringWithCString:nameBuffer encoding:NSASCIIStringEncoding];
    log->debug("[%s] after nsstring name: %s", __FUNCTION__, nsDevName);
    return nsDevName;
}

-(const NSString*)getAddress
{
    char addrBuffer[255];
    neuroDeviceGetAddress(neuroDevice, addrBuffer, sizeof(addrBuffer)/sizeof(addrBuffer[0]));
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s] before nsstring name: %s", __FUNCTION__, addrBuffer);
    auto nsDevName = [NSString stringWithCString:addrBuffer encoding:NSASCIIStringEncoding];
    log->debug("[%s] after nsstring name: %s", __FUNCTION__, nsDevName);
    return nsDevName;
}

-(int)getBatteryLevel
{
	return neuroDeviceGetBatteryLevel(neuroDevice);
}

-(void)subscribeStateChanged:(void(^)(NCDeviceState))onStatusChangedCallback
{
    auto device = *(std::shared_ptr<NeuroDevice> *)neuroDevice;
    device->subscribeStateChanged([onStatusChangedCallback](DeviceState state){
        onStatusChangedCallback((NCDeviceState)state);
    });
}

-(NCStimulationSubsystem*)getStimulationSubsystem;
{
    return [[NCStimulationSubsystem alloc] initWithNativeSubsystem:(*(std::shared_ptr<NeuroDevice> *)neuroDevice)->getStimulationSubsystem()];
}

-(NSArray<NSNumber *>*)getFeatures
{
    auto features = neuroDeviceGetFeatures(neuroDevice);
    NSMutableArray<NSNumber*>* featuresArray = [NSMutableArray array];
    for (auto it = features.begin(); it!=features.end(); ++it)
    {
        [featuresArray addObject:[NSNumber numberWithInt:(int)(*it)]];
    }
    return featuresArray;
}

@end
