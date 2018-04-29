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

#ifndef ANDROID_JNI_DEVICE_PARAM_WRAP_H
#define ANDROID_JNI_DEVICE_PARAM_WRAP_H

#include "device/device.h"
#include "device/device_parameters.h"
#include "java_environment.h"

/**
 * DeviceState enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::DeviceState>() {
    return "com/neuromd/neurosdk/parameters/types/DeviceState";
}

template<>
const std::map<Neuro::DeviceState, std::string>
jni::enum_name_map<Neuro::DeviceState>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::DeviceState>
jni::enum_name_map<Neuro::DeviceState>::mNameToEnumMap;



/**
 * Command enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::Command>() {
    return "com/neuromd/neurosdk/parameters/Command";
}

template<>
const std::map<Neuro::Command, std::string>
jni::enum_name_map<Neuro::Command>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::Command>
jni::enum_name_map<Neuro::Command>::mNameToEnumMap;



/**
 * Parameter enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::Parameter>() {
    return "com/neuromd/neurosdk/parameters/ParameterName";
}

template<>
const std::map<Neuro::Parameter, std::string>
jni::enum_name_map<Neuro::Parameter>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::Parameter>
jni::enum_name_map<Neuro::Parameter>::mNameToEnumMap;



/**
 * ParamAccess enum
 */
template<>
constexpr const char *jni::java_class_name<Neuro::ParamAccess>() {
    return "com/neuromd/neurosdk/parameters/ParameterAccess";
}

template<>
const std::map<Neuro::ParamAccess, std::string>
jni::enum_name_map<Neuro::ParamAccess>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::ParamAccess>
jni::enum_name_map<Neuro::ParamAccess>::mNameToEnumMap;



/**
 * Pair of parameter and access
 */
template<>
constexpr const char *jni::java_class_name<Neuro::ParamPair>() {
    return "com/neuromd/neurosdk/parameters/Parameter";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::ParamPair>() {
    return "(Lcom/neuromd/neurosdk/parameters/ParameterName;"
            "Lcom/neuromd/neurosdk/parameters/ParameterAccess;"
            "Lcom/neuromd/neurosdk/parameters/ParameterType;)V";
};

template<>
template<>
jni::java_object<Neuro::ParamPair>::java_object(const Neuro::ParamPair &);

jobject readDeviceParam(JNIEnv *, const Neuro::Device &, Neuro::Parameter);

jboolean setDeviceParam(JNIEnv *, Neuro::Device &, Neuro::Parameter, jobject);

#endif //ANDROID_JNI_DEVICE_PARAM_WRAP_H
