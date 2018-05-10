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
#include "stephelper_device.h"
#include "logger.h"

@implementation NCMotionAssistantDevice
{
    void *assistantDevice;
    NCNeuroDevice *neuroDevice;
}

-(id)initWithNativeDevice: (void*)device
{
    if (self = [super init]){
        assistantDevice = device;
        neuroDevice = nil;
    }
    return self;
}

-(void)dealloc
{
    if (assistantDevice != nullptr)
    {
        auto log = LoggerFactory::getCurrentPlatformLogger();
        log->debug("MA dealloc");
        auto device = (std::shared_ptr<StepHelperDevice> *)assistantDevice;
        (*device)->getNeuroDevice()->disconnect();
        delete device;
        assistantDevice = nullptr;
    }
}

-(void) motionAssistantStart
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->motionAssistantStart();
}

-(void) motionAssistantStop
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->motionAssistantStop();
}

-(bool) getMotionAssistantState
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->motionAssistantIsActive();
}

-(void) setMotionAssistantParams:(int) gyroStart gyroStop:(int)gyroStop limb:(NCMotionAssistantLimb)limb minStimulationPause:(int)minStimulationPause maxStimulationDuration:(int)maxStimulationDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    StimulationLimb cppLimb;
    parseLimbCode(limb, cppLimb);
    device->setMotionAssistantParams(gyroStart, gyroStop, cppLimb, minStimulationPause, maxStimulationDuration);
}

-(void) setGyroStartThreshold:(int) gyroStart
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setGyroStartThreshold(gyroStart);
}

-(void) setGyroStopThreshold:(int) gyroStop
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setGyroStopThreshold(gyroStop);
}

-(void) setLimbForStimulation:(NCMotionAssistantLimb) limb
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    StimulationLimb cppLimb;
    parseLimbCode(limb, cppLimb);
    device->setLimbForStimulation(cppLimb);
}

-(void) setMinAssistantStimulationPause:(int) stimulationPause
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setMinAssistantStimulationPause(stimulationPause);
}

-(void) setMaxAssistantStimulusDuration:(int) maxStimulDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setMaxAssistantStimulusDuration(maxStimulDuration);
}

-(int) getGyroStartThreshold
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getGyroStartThreshold();
}

-(int) getGyroStopThreshold
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getGyroStopThreshold();
}

-(NCMotionAssistantLimb) getLimbForStimulation
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    auto limb = device->getLimbForStimulation();
    auto limbCode = static_cast<NSUInteger>(limb);
    return (NCMotionAssistantLimb)limbCode;
}

-(int) getMinAssistantStimulationPause
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getMinAssistantStimulationPause();
}

-(int) getMaxAssistantStimulusDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getMaxAssistantStimulusDuration();
}

-(void) stimulationStart
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->stimulationStart();
}

-(void) stimulationStop
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->stimulationStop();
}

-(bool) getStimulatorState
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->stimulatorIsActive();
}

-(void) doCalibration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->doCalibration();
}

-(void) setStimulatorParams:(int) amplitude pulse_duration:(NCStimulatorImpulseDuration) pulse_duration frequency:(int) frequency stim_duration:(int) stim_duration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setStimulatorParams(amplitude, pulse_duration, frequency, stim_duration);
}

-(void) setCurrentAmplitude:(int) current
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setCurrentAmplitude(current);
}

-(void) setPulseDuration:(NCStimulatorImpulseDuration) pulseDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setPulseDuration(pulseDuration);
}

-(void) setPulseFrequency:(int) pulseFrequency
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setPulseFrequency(pulseFrequency);
}

-(void) setStimulationDuration:(int) stimulationDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setStimulationDuration(stimulationDuration);
}

-(int) getCurrentAmplitude
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getCurrentAmplitude();
}

-(NCStimulatorImpulseDuration) getPulseDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return (NCStimulatorImpulseDuration)device->getPulseDuration();
}

-(int) getPulseFrequency
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getPulseFrequency();
}

-(int) getStimulationDuration
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    return device->getStimulationDuration();
}

-(void) setAssistantParamsChangedCallback:(void(^)(int, int, NCMotionAssistantLimb, int, int)) assistantParamsChangedCallback
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setAssistantParamsChangedCallback([assistantParamsChangedCallback](int gyroStart, int gyroStop, StimulationLimb limb, int minStimulationPause, int maxStimulusDuration)
    {
        auto limbCode = static_cast<NSUInteger>(limb);
        assistantParamsChangedCallback(gyroStart, gyroStop, (NCMotionAssistantLimb)limbCode, minStimulationPause, maxStimulusDuration);
    });
}

-(void) setStimulationParamsChangedCallback:(void(^)(int, NCStimulatorImpulseDuration, int, int)) stimulParamsChangedCallback
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setStimulationParamsChangedCallback([stimulParamsChangedCallback](int amplitude, int pulse_duration, int frequency, int stimul_duration)
    {
        stimulParamsChangedCallback(amplitude, (NCStimulatorImpulseDuration)pulse_duration, frequency, stimul_duration);
    });
}

-(void) setAssistantStateChangedCallback:(void(^)(bool)) assistantStateChangedCallback
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setAssistantStateChangedCallback([assistantStateChangedCallback](bool isOn)
                                             {
                                                 assistantStateChangedCallback(isOn);
                                             });
}

-(void) setStimulatorStateChangedCallback:(void(^)(bool)) stimulatorStateChangedCallback
{
    auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
    device->setStimulatorStateChangedCallback([stimulatorStateChangedCallback](bool isOn)
                                              {
                                                  stimulatorStateChangedCallback(isOn);
                                              });
}

-(NCNeuroDevice *) getNeuroDevice
{
    if (neuroDevice == nil){
        auto device = *(std::shared_ptr<StepHelperDevice> *)assistantDevice;
        auto nativeDevice = device->getNeuroDevice();
        neuroDevice = [[NCNeuroDevice alloc] initWithNeuroDevice:new std::shared_ptr<NeuroDevice>(nativeDevice)];
    }
    return neuroDevice;
}

@end
