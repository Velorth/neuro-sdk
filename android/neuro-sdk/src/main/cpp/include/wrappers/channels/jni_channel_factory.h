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

#ifndef ANDROID_JNI_CHANNEL_FACTORY_WRAP_H
#define ANDROID_JNI_CHANNEL_FACTORY_WRAP_H

#include <android/log.h>
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"
#include "device/param_values.h"
#include "filter/digital_filter.h"
#include "filter/iir_filter.h"
#include "filter/low_pass_filter.h"


inline Neuro::ChannelInfo extractInfo(JNIEnv *env, jobject info) {
    auto objectClass = env->GetObjectClass(info);
    auto nameMethodId = env->GetMethodID(objectClass, "getName", "()Ljava/lang/String;");
    auto javaName = static_cast<jstring>(env->CallObjectMethod(info, nameMethodId));
    auto name = env->GetStringUTFChars(javaName, 0);

    auto typeSignature = std::string("()L") +
                         std::string(jni::java_class_name<Neuro::ChannelInfo::Type>()) +
                         std::string(";");
    auto typeMethodId = env->GetMethodID(objectClass, "getType", typeSignature.c_str());
    auto javaType = env->CallObjectMethod(info, typeMethodId);
    auto type = jni::enumFromJavaObj<Neuro::ChannelInfo::Type>(env, javaType);

    auto indexMethodId = env->GetMethodID(objectClass, "getIndex", "()J");
    auto index = env->CallLongMethod(info, indexMethodId);

    Neuro::ChannelInfo channelInfo(type, name, saturation_cast<std::size_t>(index));
    env->ReleaseStringUTFChars(javaName, name);
    return channelInfo;
}

inline DSP::DigitalFilterPtr<double> filterFromName(const std::string &filter_name){
    if (filter_name == "LowPass_27_SF125") {
        return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<27, 5, 125>>>();
    }
    else if (filter_name == "LowPass_1_SF125_Reverse"){
        return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<1, 3, 125>>>();
    }
    else {
        throw std::runtime_error("Filter is not supported");
    }
}

inline std::vector<DSP::DigitalFilterPtr<double>> createFilters(JNIEnv *env, jobjectArray filterNames) {
    auto commandEnumClass = env->FindClass("com/neuromd/neurosdk/channels/Filter");
    auto nameMethodID = env->GetMethodID(commandEnumClass, "name", "()Ljava/lang/String;");
    auto count = env->GetArrayLength(filterNames);
    std::vector<DSP::DigitalFilterPtr<double>> filters;
    for (auto i = 0; i < count; ++i){
        auto enumObj = env->GetObjectArrayElement(filterNames, i);
        auto nameString = static_cast<jstring>(env->CallObjectMethod(enumObj, nameMethodID));
        auto enumName = env->GetStringUTFChars(nameString, nullptr);
        filters.push_back(filterFromName(enumName));
        env->ReleaseStringUTFChars(nameString, enumName);
        env->DeleteLocalRef(nameString);
    }
    return filters;
}


template <typename ChannelWrap>
jlong createChannelFromDevice(JNIEnv *env, jobject device){
    auto& deviceWrapPtr = *extract_pointer<JniDeviceWrap>(env, device);
    try {
        auto channel = std::make_shared<typename ChannelWrap::obj_t>(*deviceWrapPtr);
        auto channelWrap = new ChannelWrap(channel);
        return reinterpret_cast<jlong>(channelWrap);
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_ERROR, "CreateChannelFromDevice",
                            "Error creating channel: %s", e.what());
        jni::java_throw(env, "java/lang/IllegalArgumentException", e);
        return 0;
    }
}

template <typename ChannelWrap>
jlong createChannelFromDevice(JNIEnv *env, jobject device, jobject info){
    auto &deviceWrapPtr = *extract_pointer<JniDeviceWrap>(env, device);
    auto channelInfo = extractInfo(env, info);
    try{
        auto channel = std::make_shared<typename ChannelWrap::obj_t>(*deviceWrapPtr, channelInfo);
        auto channelWrap = new ChannelWrap(channel);
        return reinterpret_cast<jlong>(channelWrap);
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_ERROR, "CreateChannelFromDevice",
                            "Error creating channel: %s", e.what());
        jni::java_throw(env, "java/lang/IllegalArgumentException", e);
        return 0;
    }
}

template <typename ChannelWrap>
jlong createChannelFromDevice(JNIEnv *env, jobject device, jobjectArray filterNames){
    auto &deviceWrapPtr = *extract_pointer<JniDeviceWrap>(env, device);
    try{
        auto filters = createFilters(env, filterNames);
        auto channel = std::make_shared<typename ChannelWrap::obj_t>(*deviceWrapPtr, std::move(filters));
        auto channelWrap = new ChannelWrap(channel);
        return reinterpret_cast<jlong>(channelWrap);
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_ERROR, "CreateChannelFromDevice",
                            "Error creating channel: %s", e.what());
        jni::java_throw(env, "java/lang/IllegalArgumentException", e);
        return 0;
    }
}

template <typename ChannelWrap>
jlong createChannelFromDevice(JNIEnv *env, jobject device, jobject info, jobjectArray filterNames){
    auto &deviceWrapPtr = *extract_pointer<JniDeviceWrap>(env, device);
    try{
        auto channelInfo = extractInfo(env, info);
        auto filters = createFilters(env, filterNames);
        auto channel = std::make_shared<typename ChannelWrap::obj_t>(*deviceWrapPtr, std::move(filters), channelInfo);
        auto channelWrap = new ChannelWrap(channel);
        return reinterpret_cast<jlong>(channelWrap);
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_ERROR, "CreateChannelFromDevice",
                            "Error creating channel: %s", e.what());
        jni::java_throw(env, "java/lang/IllegalArgumentException", e);
        return 0;
    }
}

#endif //ANDROID_JNI_CHANNEL_FACTORY_WRAP_H
