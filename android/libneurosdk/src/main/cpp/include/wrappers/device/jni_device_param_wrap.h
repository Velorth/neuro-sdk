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

#ifndef ANDROID_JNI_DEVICE_PARAM_WRAP_H
#define ANDROID_JNI_DEVICE_PARAM_WRAP_H

#include "device/device_parameters.h"
#include "java_environment.h"

template<>
constexpr const char *jni::java_class_name<Neuro::DeviceState>() {
    return "ru/neurotech/neurosdk/parameters/types/DeviceState";
}

template<>
template<>
jni::java_object<Neuro::DeviceState>::java_object(const Neuro::DeviceState &);

template<>
constexpr const char *jni::java_class_name<Neuro::Command>() {
    return "ru/neurotech/neurosdk/parameters/Command";
}

template<>
template<>
jni::java_object<Neuro::Command>::java_object(const Neuro::Command &);

template<>
constexpr const char *jni::java_class_name<Neuro::Parameter>() {
    return "ru/neurotech/neurosdk/parameters/ParameterName";
}

template<>
template<>
jni::java_object<Neuro::Parameter>::java_object(const Neuro::Parameter &);

template<>
constexpr const char *jni::java_class_name<Neuro::ParamAccess>() {
    return "ru/neurotech/neurosdk/parameters/ParameterAccess";
}

template<>
template<>
jni::java_object<Neuro::ParamAccess>::java_object(const Neuro::ParamAccess &);

template<>
constexpr const char *jni::java_class_name<Neuro::ParamPair>() {
    return "ru/neurotech/neurosdk/parameters/Parameter";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::ParamPair>() {
    return "(Lru/neurotech/neurosdk/parameters/ParameterName;"
            "Lru/neurotech/neurosdk/parameters/ParameterAccess;"
            "Lru/neurotech/neurosdk/parameters/ParameterType;)V";
};

template<>
template<>
jni::java_object<Neuro::ParamPair>::java_object(const Neuro::ParamPair &);

#endif //ANDROID_JNI_DEVICE_PARAM_WRAP_H
