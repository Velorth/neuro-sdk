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
#include "stimulation_subsystem.h"

@implementation NCStimulationSubsystem
{
    StimulationSubsystem* stimulSubsystem;
}

-(id)initWithNativeSubsystem: (void*)subsystem;
{
    if (self = [super init])
    {
        stimulSubsystem = (StimulationSubsystem*)subsystem;
    }
    return self;
}

-(void)stimulStart
{
    stimulSubsystem->stimulStart();
}

-(void)stimulStop
{
    stimulSubsystem->stimulStop();
}

-(bool)getStimulatorIsActive
{
    return stimulSubsystem->getStimulatorIsActive();
}

-(void)setStimulParams:(int) current pulse_duration:(int)pulse_duration frequency:(int)frequency stimulus_duration:(int)stimulus_duration
{
    stimulSubsystem->setStimulParams(current, pulse_duration, frequency, stimulus_duration);
}

-(int)getCurrentAmplitude
{
    return stimulSubsystem->getCurrentAmplitude();
}

-(int)getPulseDuration
{
    return stimulSubsystem->getPulseDuration();
}

-(int)getPulseFrequency
{
    return stimulSubsystem->getPulseFrequency();
}

-(int)getStimulationDuration
{
    return stimulSubsystem->getStimulationDuration();
}

-(void)shStart
{
    stimulSubsystem->shStart();
}

-(void)shStop
{
    stimulSubsystem->shStop();
}

-(bool)getShIsActive
{
    return stimulSubsystem->getShIsActive();
}

-(void)setShParams:(int) gyroStart gyroStop:(int)gyroStop limb:(NCMotionAssistantLimb)limb stimulationPause:(int)stimulationPause maxStimulationDuration:(int)maxStimulationDuration
{
    StimulationLimb cppLimb;
    parseLimbCode(limb, cppLimb);
    stimulSubsystem->setShParams(gyroStart, gyroStop, cppLimb, stimulationPause, maxStimulationDuration);
}

-(int)getGyroStart
{
    return stimulSubsystem->getGyroStart();
}

-(int)getGyroStop
{
    return stimulSubsystem->getGyroStop();
}

-(NCMotionAssistantLimb)getSelectedLimb
{
    auto limb = stimulSubsystem->getSelectedLimb();
    auto limbCode = static_cast<NSUInteger>(limb);
    return (NCMotionAssistantLimb)limbCode;
}

-(int)getStimulationPause
{
    return stimulSubsystem->getStimulationPause();
}

-(int)getMaxStimulationDuration
{
    return stimulSubsystem->getMaxStimulationDuration();
}

-(void)doCalibration
{
    stimulSubsystem->doCalibration();
}

@end
