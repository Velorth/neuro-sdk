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

#ifndef ANDROID_JNI_BFB_DEVICE_WRAP_H
#define ANDROID_JNI_BFB_DEVICE_WRAP_H

#include "jni_ptr_wrap.h"
#include "method/bfb_device.h"

class JniBfbDevice : public JniDeviceWrap<BfbDevice>{
public:
    JniBfbDevice(std::shared_ptr<BfbDevice> devicePtr) : JniDeviceWrap(devicePtr){}
    ~JniBfbDevice() = default;
};

class JniBfbIndex : public JniPtrWrap<BfbIndex<>> {
public:
    JniBfbIndex(std::shared_ptr<BfbIndex<>> indexPtr) : JniPtrWrap(indexPtr) {}
    ~JniBfbIndex() = default;

    void subscribeIndexValueChanged(jobject subscriberRef);
    void subscribeCalibrationFinished(jobject subscriberRef);
private:
    std::shared_ptr<jni::jobject_t> indexValueChangedSubscriberRef;
    std::shared_ptr<jni::jobject_t> calibrationFinishedSubscriberRef;

    EventHandler(BfbIndex<>, indexValueChanged) indexValueChangedHandler;
    EventHandler(BfbIndex<>, calibrationFinished) calibrationFinishedHandler;
};

template<>
constexpr const char *jni::java_class_name<JniBfbDevice*>() { return "ru/neurotech/neurodevices/bfb/BfbDevice"; };

template<>
constexpr const char *jni::constructor_signature<JniBfbDevice*>() { return "(J)V"; };

template<>
constexpr const char *jni::java_class_name<JniBfbIndex*>() { return "ru/neurotech/neurodevices/bfb/BfbIndex"; };

template<>
constexpr const char *jni::constructor_signature<JniBfbIndex*>() { return "(J)V"; };

#endif //ANDROID_JNI_BFB_DEVICE_WRAP_H
