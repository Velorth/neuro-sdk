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

#ifndef ANDROID_JNI_BASE_CHANNEL_WRAP_H
#define ANDROID_JNI_BASE_CHANNEL_WRAP_H

#include "wrappers/jni_ptr_wrap.h"
#include "wrappers/jni_device_wrap.h"
#include "channels/base_channel.h"

class JniBaseChannelWrap : public JniPtrWrap<Neuro::BaseChannel> {
public:
    void subscribeLengthChanged(jobject stateChangedSubscriberRef);

protected:
    JniBaseChannelWrap(object_ptr_t devicePtr) :
            JniPtrWrap<Neuro::BaseChannel>(devicePtr) {}

private:
    std::shared_ptr<jni::jobject_t> lengthChangedGlobalSubscriberRef;
};

template<>
constexpr const char *jni::java_class_name<JniBaseChannelWrap *>() {
    return "ru/neurotech/neurosdk/channels/NativeChannel";
}

template<>
constexpr const char *jni::constructor_signature<JniBaseChannelWrap *>() {
    return "(J)V";
}

template <typename ChannelWrap>
jlong createChannelFromDevice(JNIEnv *env, jobject device){
    auto deviceWrapPtr = *extract_pointer<JniDeviceWrap>(env, device);
    try {
        auto batteryChannel = std::make_shared<typename ChannelWrap::obj_t>(*deviceWrapPtr);
        auto batteryChannelWrap = new ChannelWrap(batteryChannel);
        return reinterpret_cast<jlong>(batteryChannelWrap);
    }
    catch (std::exception &e){
        auto exceptionClass = env->FindClass("java/lang/IllegalArgumentException");
        if (exceptionClass == nullptr) {
            return 0;
        }
        env->ThrowNew(exceptionClass, e.what());
        return 0;
    }
}

#endif //ANDROID_JNI_BASE_CHANNEL_WRAP_H
