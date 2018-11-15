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

#ifndef ANDROID_JNI_SIGNAL_CHANNEL_WRAP_H
#define ANDROID_JNI_SIGNAL_CHANNEL_WRAP_H

#include "wrappers/jni_ptr_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"
#include "device/device.h"
#include "channels/device_channel.h"

class JniSignalChannelWrap : public JniPtrWrap<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> {
public:
    void subscribeLengthChanged(jobject stateChangedSubscriberRef);
public:
    JniSignalChannelWrap(object_ptr_t devicePtr) :
            JniPtrWrap(devicePtr) {}
private:
    std::shared_ptr<jni::jobject_t> lengthChangedGlobalSubscriberRef;
    Neuro::ListenerPtr<void, Neuro::data_length_t> mListener;
};

template<>
constexpr const char *jni::java_class_name<JniSignalChannelWrap *>() {
    return "com/neuromd/neurosdk/channels/SignalChannel";
}

template<>
constexpr const char *jni::constructor_signature<JniSignalChannelWrap *>() {
    return "(J)V";
}

#endif //ANDROID_JNI_SIGNAL_CHANNEL_WRAP_H
