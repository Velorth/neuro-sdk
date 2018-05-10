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

#ifndef ANDROID_JNI_CHANNEL_INFO_WRAP_H
#define ANDROID_JNI_CHANNEL_INFO_WRAP_H

#include "channels/channel_info.h"
#include "java_environment.h"

template<>
constexpr const char *jni::java_class_name<Neuro::ChannelInfo::Type>() {
    return "com/neuromd/neurosdk/channels/ChannelType";
}

template<>
const std::map<Neuro::ChannelInfo::Type, std::string>
jni::enum_name_map<Neuro::ChannelInfo::Type>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::ChannelInfo::Type>
jni::enum_name_map<Neuro::ChannelInfo::Type>::mNameToEnumMap;



template<>
constexpr const char *jni::java_class_name<Neuro::ChannelInfo *>() {
    return "com/neuromd/neurosdk/channels/NativeChannelInfo";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::ChannelInfo *>() {
    return "(J)V";
}



template<>
constexpr const char *jni::java_class_name<Neuro::ChannelInfo>() {
    return "com/neuromd/neurosdk/channels/ManagedChannelInfo";
}

template<>
constexpr const char *jni::constructor_signature<Neuro::ChannelInfo>() {
    return "(Lcom/neuromd/neurosdk/channels/ChannelType;Ljava/lang/String;)V";
}

template<>
template<>
jni::java_object<Neuro::ChannelInfo>::java_object(const Neuro::ChannelInfo &);

#endif //ANDROID_JNI_CHANNEL_INFO_WRAP_H
