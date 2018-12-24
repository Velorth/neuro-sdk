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

#ifndef ANDROID_JNI_PARAM_TYPE_WRAP_H
#define ANDROID_JNI_PARAM_TYPE_WRAP_H

#include "device/device_parameters.h"
#include "java_environment.h"

/**
 * Stimulation parameter type
 */

template<>
constexpr const char *jni::java_class_name<Neuro::StimulationParams>() {
    return "com/neuromd/neurosdk/parameters/types/StimulationParams";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::StimulationParams>() {
    return "(IIII)V";
}

template<>
template<>
jni::java_object<Neuro::StimulationParams>::java_object(const Neuro::StimulationParams &);

template<>
Neuro::StimulationParams jni::get_java_obj_value<Neuro::StimulationParams>(JNIEnv *, jobject);

/**
 * StimulatorDeviceState parameter type
 */

template<>
constexpr const char *jni::java_class_name<Neuro::StimulatorDeviceState>() {
    return "com/neuromd/neurosdk/parameters/types/StimulatorDeviceState";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::StimulatorDeviceState>() {
    return "(Lcom/neuromd/neurosdk/parameters/types/StimulatorDeviceState$State;Lcom/neuromd/neurosdk/parameters/types/StimulatorDeviceState$State;)V";
}

template<>
template<>
jni::java_object<Neuro::StimulatorDeviceState>::java_object(const Neuro::StimulatorDeviceState &);

/**
 * MotionAssistant parameter type
 */

template<>
constexpr const char *jni::java_class_name<Neuro::MotionAssistantParams>() {
    return "com/neuromd/neurosdk/parameters/types/MotionAssistantParams";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::MotionAssistantParams>() {
    return "(IILcom/neuromd/neurosdk/parameters/types/MotionAssistantLimb;I)V";
}

template<>
template<>
jni::java_object<Neuro::MotionAssistantParams>::java_object(const Neuro::MotionAssistantParams &);

template<>
Neuro::MotionAssistantParams jni::get_java_obj_value<Neuro::MotionAssistantParams>(JNIEnv *, jobject);


/**
 * FirmwareVersion parameter type
 */

template<>
constexpr const char *jni::java_class_name<Neuro::FirmwareVersion>() {
    return "com/neuromd/neurosdk/parameters/types/FirmwareVersion";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::FirmwareVersion>() {
    return "(II)V";
}

template<>
template<>
jni::java_object<Neuro::FirmwareVersion>::java_object(const Neuro::FirmwareVersion &);


template<>
Neuro::FirmwareVersion jni::get_java_obj_value<Neuro::FirmwareVersion>(JNIEnv *, jobject);

/**
 * ADCInput enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::ADCInput>() {
    return "com/neuromd/neurosdk/parameters/types/ADCInput";
}

template<>
const std::map<Neuro::ADCInput, std::string>
        jni::enum_name_map<Neuro::ADCInput>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::ADCInput>
        jni::enum_name_map<Neuro::ADCInput>::mNameToEnumMap;



/**
 * Accelerometer sens enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::AccelerometerSensitivity>() {
    return "com/neuromd/neurosdk/parameters/types/AccelerometerSensitivity";
}

template<>
const std::map<Neuro::AccelerometerSensitivity , std::string>
        jni::enum_name_map<Neuro::AccelerometerSensitivity>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::AccelerometerSensitivity >
        jni::enum_name_map<Neuro::AccelerometerSensitivity>::mNameToEnumMap;



/**
 * Gyro sens enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::GyroscopeSensitivity>() {
    return "com/neuromd/neurosdk/parameters/types/GyroscopeSensitivity";
}

template<>
const std::map<Neuro::GyroscopeSensitivity , std::string>
        jni::enum_name_map<Neuro::GyroscopeSensitivity>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::GyroscopeSensitivity >
        jni::enum_name_map<Neuro::GyroscopeSensitivity>::mNameToEnumMap;



/**
 * ExternalSwitchInput enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::ExternalSwitchInput>() {
    return "com/neuromd/neurosdk/parameters/types/ExternalSwitchInput";
}

template<>
const std::map<Neuro::ExternalSwitchInput, std::string>
        jni::enum_name_map<Neuro::ExternalSwitchInput>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::ExternalSwitchInput>
        jni::enum_name_map<Neuro::ExternalSwitchInput>::mNameToEnumMap;



/**
 * FirmwareMode enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::FirmwareMode>() {
    return "com/neuromd/neurosdk/parameters/types/FirmwareMode";
}

template<>
const std::map<Neuro::FirmwareMode, std::string>
        jni::enum_name_map<Neuro::FirmwareMode>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::FirmwareMode>
        jni::enum_name_map<Neuro::FirmwareMode>::mNameToEnumMap;



/**
 * Gain enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::Gain>() {
    return "com/neuromd/neurosdk/parameters/types/Gain";
}

template<>
const std::map<Neuro::Gain, std::string>
        jni::enum_name_map<Neuro::Gain>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::Gain>
        jni::enum_name_map<Neuro::Gain>::mNameToEnumMap;



/**
 * MotionAssistantLimb enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::MotionAssistantLimb>() {
    return "com/neuromd/neurosdk/parameters/types/MotionAssistantLimb";
}

template<>
const std::map<Neuro::MotionAssistantLimb, std::string>
        jni::enum_name_map<Neuro::MotionAssistantLimb>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::MotionAssistantLimb>
        jni::enum_name_map<Neuro::MotionAssistantLimb>::mNameToEnumMap;





/**
 * StimulatorDeviceState enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::StimulatorDeviceState::State>() {
    return "com/neuromd/neurosdk/parameters/types/StimulatorDeviceState$State";
}

template<>
const std::map<Neuro::StimulatorDeviceState::State, std::string>
        jni::enum_name_map<Neuro::StimulatorDeviceState::State>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::StimulatorDeviceState::State>
        jni::enum_name_map<Neuro::StimulatorDeviceState::State>::mNameToEnumMap;


/**
 * SamplingFrequency enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::SamplingFrequency>() {
    return "com/neuromd/neurosdk/parameters/types/SamplingFrequency";
}

template<>
const std::map<Neuro::SamplingFrequency, std::string>
        jni::enum_name_map<Neuro::SamplingFrequency>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::SamplingFrequency>
        jni::enum_name_map<Neuro::SamplingFrequency>::mNameToEnumMap;



std::string getParamTypeName(Neuro::Parameter);

class ParameterType{
public:
    ParameterType(Neuro::Parameter);
    std::string name() const;
private:
    std::string mName;
};

template<>
constexpr const char *jni::java_class_name<ParameterType>() {
    return "com/neuromd/neurosdk/parameters/ParameterType";
}

template<>
template<>
jni::java_object<ParameterType>::java_object(const ParameterType &);

#endif //ANDROID_JNI_PARAM_TYPE_WRAP_H
