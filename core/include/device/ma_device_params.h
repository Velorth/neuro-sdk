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

#ifndef STIMULATION_SUBSYSTEM_H
#define STIMULATION_SUBSYSTEM_H

namespace Neuro {

enum class MotionAssistantLimb: unsigned char{

    RIGHT_LEG = 0,
    LEFT_LEG = 1,
    RIGHT_ARM = 2,
    LEFT_ARM = 3
};

inline bool parseLimbCode(unsigned char limbByte, MotionAssistantLimb &limb){

    switch (limbByte){

        case static_cast<unsigned char>(MotionAssistantLimb::LEFT_ARM):{
            limb = MotionAssistantLimb::LEFT_ARM;
            return true;
        }
        case static_cast<unsigned char>(MotionAssistantLimb::LEFT_LEG):{
            limb = MotionAssistantLimb::LEFT_LEG;
            return true;
        }
        case static_cast<unsigned char>(MotionAssistantLimb::RIGHT_ARM):{
            limb = MotionAssistantLimb::RIGHT_ARM;
            return true;
        }
        case static_cast<unsigned char>(MotionAssistantLimb::RIGHT_LEG):{
            limb = MotionAssistantLimb::RIGHT_LEG;
            return true;
        }
        default:
            return false;
    }
}

struct MotionAssistantParams {
    int gyroStart;
    int gyroStop;
    MotionAssistantLimb limb;
    int minPause;
    int maxDuration;
};

struct StimulationParams {
    int current;
    int pulse_duration;
    int frequency;
    int stimulus_duration;
};

}
#endif //STIMULATION_SUBSYSTEM_H
