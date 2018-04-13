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

#ifndef ANDROID_JNI_EEG_DEVICE_WRAP_H
#define ANDROID_JNI_EEG_DEVICE_WRAP_H

#include "method/eeg_device.h"
#include "jni_ptr_wrap.h"

class JniEegDevice : public JniDeviceWrap<EegDevice>{
public:
    JniEegDevice(std::shared_ptr<EegDevice> devicePtr) : JniDeviceWrap(devicePtr){}
    ~JniEegDevice() = default;

    /*void subscribeTotalSignalDurationChanged(jobject subscriberRef);
    void subscribeElectrodesAttachedStateChanged(jobject subscriberRef);
    void subscribeHeartRateChanged(jobject subscriberRef);
    void subscribeStressIndexChanged(jobject subscriberRef);

private:
    EventHandler(EegDevice, motionAssistantParamsChanged) motionAssistantParamsChangedHandler;
    EventHandler(EegDevice, isElectrodesAttachedChanged) isElectrodeAttachedHandler;
    EventHandler(EegDevice, heartRateChanged) heartRateChangedHandler;
    EventHandler(EegDevice, stressIndexChanged) stressIndexChangedHandler;*/
};

template<>
constexpr const char *jni::java_class_name<JniEegDevice*>() { return "ru/neurotech/neurodevices/eeg/EegDevice"; };

template<>
constexpr const char *jni::constructor_signature<JniEegDevice*>() { return "(J)V"; };

#endif //ANDROID_JNI_EEG_DEVICE_WRAP_H
