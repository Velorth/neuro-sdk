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

#ifndef STIMULATION_SUBSYSTEM_H
#define STIMULATION_SUBSYSTEM_H

namespace Neuro {

enum class MotionAssistantLimb {
    RightLeg = 0,
    LeftLeg = 1,
    RightArm = 2,
    LeftArm = 3
};

enum class StimulatorImpulseDuration {
    us60 = 6,
    us100 = 10,
    us200 = 20
};

inline bool parseLimbCode(unsigned char limbByte, MotionAssistantLimb &limb){

    switch (limbByte){

        case static_cast<unsigned char>(MotionAssistantLimb::LeftArm):{
            limb = MotionAssistantLimb::LeftArm;
            return true;
        }
        case static_cast<unsigned char>(MotionAssistantLimb::LeftLeg):{
            limb = MotionAssistantLimb::LeftLeg;
            return true;
        }
        case static_cast<unsigned char>(MotionAssistantLimb::RightArm):{
            limb = MotionAssistantLimb::RightArm;
            return true;
        }
        case static_cast<unsigned char>(MotionAssistantLimb::RightLeg):{
            limb = MotionAssistantLimb::RightLeg;
            return true;
        }
        default:
            return false;
    }
}

inline bool parseImpulseDuration(unsigned char impulseDurationValue, StimulatorImpulseDuration &duration){

    switch (impulseDurationValue){

        case static_cast<unsigned char>(StimulatorImpulseDuration::us60):{
            duration = StimulatorImpulseDuration::us60;
            return true;
        }
        case static_cast<unsigned char>(StimulatorImpulseDuration::us100):{
            duration = StimulatorImpulseDuration::us100;
            return true;
        }
        case static_cast<unsigned char>(StimulatorImpulseDuration::us200):{
            duration = StimulatorImpulseDuration::us200;
            return true;
        }
        default:
            return false;
    }
}

struct StimulatorDeviceState{
    enum class State{
        NoParams = 0,
        Disabled = 1,
        Enabled = 2
    };

    State StimulatorState;
    State MAState;
};

struct MotionAssistantParams {
    int gyroStart;
    int gyroStop;
    MotionAssistantLimb limb;
    int minPause;
    int maxDuration;
};

struct StimulationParams {
    int current;
    StimulatorImpulseDuration pulse_duration;
    int frequency;
    int stimulus_duration;
};

}
#endif //STIMULATION_SUBSYSTEM_H
