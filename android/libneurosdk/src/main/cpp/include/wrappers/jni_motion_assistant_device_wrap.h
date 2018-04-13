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

#ifndef ANDROID_JNI_MA_DEVICE_WRAP_H
#define ANDROID_JNI_MA_DEVICE_WRAP_H

#include "method/ma_device.h"
#include "jni_ptr_wrap.h"

template<>
constexpr const char *jni::java_class_name<MotionAssistantLimb>() { return "ru/neurotech/neurodevices/stimulation/MotionAssistantLimb"; };

template<>
constexpr const char *jni::constructor_signature<MotionAssistantLimb>() { return "(I)Lru/neurotech/neurodevices/stimulation/MotionAssistantLimb;"; };

template<>
template<>
jni::java_object<MotionAssistantLimb>::java_object(const MotionAssistantLimb &);

template<>
constexpr const char *jni::java_class_name<MotionAssistantParams>() { return "ru/neurotech/neurodevices/stimulation/MotionAssistantParams"; };

template<>
constexpr const char *jni::constructor_signature<MotionAssistantParams>() { return "()V"; };

template<>
template<>
jni::java_object<MotionAssistantParams>::java_object(const MotionAssistantParams &);

template<>
constexpr const char *jni::java_class_name<StimulationParams>() { return "ru/neurotech/neurodevices/stimulation/StimulationParams"; };

template<>
constexpr const char *jni::constructor_signature<StimulationParams>() { return "()V"; };

template<>
template<>
jni::java_object<StimulationParams>::java_object(const StimulationParams &);


class JniMotionAssistantDevice : public JniDeviceWrap<MotionAssistantDevice>{
public:
    JniMotionAssistantDevice(std::shared_ptr<MotionAssistantDevice> devicePtr) : JniDeviceWrap(devicePtr){}
    ~JniMotionAssistantDevice() = default;

    void subscribeMotionAssistantParamsChanged(jobject instance);
    void subscribeMotionAssistantStateChanged(jobject instance);
    void subscribeStimulationParamsChanged(jobject instance);
    void subscribeStimulatorStateChanged(jobject instance);

private:
    std::shared_ptr<jni::jobject_t> motionAssistantParamsChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> motionAssistantStateChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> stimulationParamsChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> stimulatorStateChangedSubscriberRef;

    EventHandler(MotionAssistantDevice, motionAssistantParamsChanged) motionAssistantParamsChangedHandler;
    EventHandler(MotionAssistantDevice, motionAssistantStateChanged) motionAssistantStateChangedHandler;
    EventHandler(MotionAssistantDevice, stimulationParamsChanged) stimulationParamsChangedHandler;
    EventHandler(MotionAssistantDevice, stimulatorStateChanged) stimulatorStateChangedHandler;
};

template<>
constexpr const char *jni::java_class_name<JniMotionAssistantDevice*>() { return "ru/neurotech/neurodevices/stimulation/MotionAssistantDevice"; };

template<>
constexpr const char *jni::constructor_signature<JniMotionAssistantDevice*>() { return "(J)V"; };

#endif //ANDROID_JNI_MA_DEVICE_WRAP_H
