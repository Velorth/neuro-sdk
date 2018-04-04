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

#ifndef ANDROID_JNI_CHANNEL_FACTORY_WRAP_H
#define ANDROID_JNI_CHANNEL_FACTORY_WRAP_H

#include <android/log.h>
#include "wrappers/device/jni_device_wrap.h"
#include "device/param_values.h"

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
    auto& deviceWrapPtr = *extract_pointer<JniDeviceWrap>(env, device);
    auto infoPtr = extract_pointer<Neuro::ChannelInfo>(env, info);
    if (infoPtr == nullptr){
        __android_log_print(ANDROID_LOG_ERROR, "CreateChannelFromDevice",
                            "Error creating channel: ChannelInfo object does not have native pointer");
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("ChannelInfo object does not have native pointer"));
        return 0;
    }
    try {
        auto channel = std::make_shared<typename ChannelWrap::obj_t>(*deviceWrapPtr, *infoPtr);
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
