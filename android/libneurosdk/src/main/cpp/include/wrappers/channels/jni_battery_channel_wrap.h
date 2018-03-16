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

#ifndef ANDROID_JNI_BATTERY_CHANNEL_WRAP_H
#define ANDROID_JNI_BATTERY_CHANNEL_WRAP_H

#include "wrappers/channels/jni_base_channel_wrap.h"
#include "channels/battery_channel.h"

class JniBatteryChannelWrap : public JniBaseChannelWrap {
public:
    JniBatteryChannelWrap(object_ptr_t devicePtr) :
            JniBaseChannelWrap(devicePtr) {}
};

template<>
constexpr const char *jni::java_class_name<JniBatteryChannelWrap *>() {
    return "ru/neurotech/neurosdk/channels/BatteryChannel";
}

template<>
constexpr const char *jni::constructor_signature<JniBatteryChannelWrap *>() {
    return "(J)V";
}

#endif //ANDROID_JNI_BATTERY_CHANNEL_WRAP_H
