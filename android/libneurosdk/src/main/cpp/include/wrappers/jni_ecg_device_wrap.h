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

#ifndef ANDROID_JNI_ECG_DEVICE_WRAP_H
#define ANDROID_JNI_ECG_DEVICE_WRAP_H

#include "jni_ptr_wrap.h"
#include "method/ecg_device.h"

template<>
constexpr const char *jni::java_class_name<RPeak>() { return "ru/neurotech/neurodevices/ecg/RPeak"; };

template<>
constexpr const char *jni::constructor_signature<RPeak>() { return "(DDD)V"; };

template<>
template<>
jni::java_object<RPeak>::java_object(const RPeak &);

class JniEcgDevice : public JniDeviceWrap<EcgDevice>{
public:
    JniEcgDevice(std::shared_ptr<EcgDevice> devicePtr) : JniDeviceWrap(devicePtr){}
    ~JniEcgDevice() = default;

    void subscribeTotalSignalDurationChanged(jobject subscriberRef);
    void subscribeElectrodesAttachedStateChanged(jobject subscriberRef);
    void subscribeHeartRateChanged(jobject subscriberRef);
    void subscribeStressIndexChanged(jobject subscriberRef);

private:
    std::shared_ptr<jni::jobject_t> totalSignalDurationChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> electrodesAttachedStateChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> heartRateChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> stressIndexChangedSubscriberRef;

    EventHandler(EcgDevice, ecgSignalDurationChanged) ecgSignalDurationChangedHandler;
    EventHandler(EcgDevice, isElectrodesAttachedChanged) isElectrodeAttachedHandler;
    EventHandler(EcgDevice, heartRateChanged) heartRateChangedHandler;
    EventHandler(EcgDevice, stressIndexChanged) stressIndexChangedHandler;
};

template<>
constexpr const char *jni::java_class_name<JniEcgDevice*>() { return "ru/neurotech/neurodevices/ecg/EcgDevice"; };

template<>
constexpr const char *jni::constructor_signature<JniEcgDevice*>() { return "(J)V"; };

#endif //ANDROID_JNI_ECG_DEVICE_WRAP_H
