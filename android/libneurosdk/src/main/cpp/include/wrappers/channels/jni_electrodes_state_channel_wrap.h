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

#ifndef ANDROID_JNI_ELECTRODES_STATE_CHANNEL_WRAP_H
#define ANDROID_JNI_ELECTRODES_STATE_CHANNEL_WRAP_H

#include "wrappers/jni_ptr_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"
#include "channels/electrode_state_channel.h"

class JniElectrodesStateChannelWrap : public JniPtrWrap<Neuro::ElectrodeStateChannel> {
public:
    void subscribeLengthChanged(jobject stateChangedSubscriberRef);
public:
    JniElectrodesStateChannelWrap(object_ptr_t devicePtr) :
            JniPtrWrap(devicePtr) {}
private:
    static constexpr const char *class_name = "JniElectrodesStateChannelWrap";
    std::shared_ptr<jni::jobject_t> lengthChangedGlobalSubscriberRef;
    Neuro::ListenerPtr<void, Neuro::data_length_t> mListener;
};

template<>
constexpr const char *jni::java_class_name<JniElectrodesStateChannelWrap *>() {
    return "com/neuromd/neurosdk/channels/ElectrodesStateChannel";
}

template<>
constexpr const char *jni::constructor_signature<JniElectrodesStateChannelWrap *>(){
    return "(J)V";
}

template<>
constexpr const char *jni::java_class_name<Neuro::ElectrodeState>() {
    return "com/neuromd/neurosdk/channels/ElectrodeState";
}

template<>
const std::map<Neuro::ElectrodeState, std::string>
        jni::enum_name_map<Neuro::ElectrodeState>::mEnumToNameMap;

template<>
const std::map<std::string, Neuro::ElectrodeState>
        jni::enum_name_map<Neuro::ElectrodeState>::mNameToEnumMap;

#endif //ANDROID_JNI_ELECTRODES_STATE_CHANNEL_WRAP_H