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

package ru.neurotech.neurosdk.stimulation;

public enum MotionAssistantLimb {
    RIGHT_LEG(0),
    LEFT_LEG(1),
    RIGHT_ARM(2),
    LEFT_ARM(3);

    private final int mLimbCode;

    MotionAssistantLimb(int limbCode) {
        mLimbCode = limbCode;
    }

    public static MotionAssistantLimb fromIntCode(int limbCode) {
        for (MotionAssistantLimb stateField : MotionAssistantLimb.values()) {
            if (limbCode == stateField.mLimbCode)
                return stateField;
        }
        return RIGHT_LEG;
    }

    public int getCode(){
        return mLimbCode;
    }
}
