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

#ifndef ANDROID_JNI_NEURO_DEVICE_WRAP_H
#define ANDROID_JNI_NEURO_DEVICE_WRAP_H

#include "device/device.h"
#include "wrappers/jni_ptr_wrap.h"

class JniDeviceWrap : public JniPtrWrap<Neuro::Device> {
public:
    JniDeviceWrap(object_ptr_t devicePtr) : JniPtrWrap<Neuro::Device>(devicePtr) {}

    void subscribeParameterChanged(jobject stateChangedSubscriberRef);

private:
    std::shared_ptr<jni::jobject_t> parameterChangedGlobalSubscriberRef;
};

template<>
constexpr const char *jni::java_class_name<JniDeviceWrap*>() { return "com/neuromd/neurosdk/Device"; };

template<>
constexpr const char *jni::constructor_signature<JniDeviceWrap*>() { return "(J)V"; };

#endif //ANDROID_JNI_NEURO_DEVICE_WRAP_H
