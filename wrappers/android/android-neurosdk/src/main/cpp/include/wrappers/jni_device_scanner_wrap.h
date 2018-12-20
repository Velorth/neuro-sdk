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

#ifndef ANDROID_JNI_NEURO_DEVICE_SCANNER_WRAP_H
#define ANDROID_JNI_NEURO_DEVICE_SCANNER_WRAP_H

#include "device_scanner/scanner_factory.h"
#include "wrappers/jni_ptr_wrap.h"

class JniDeviceScannerWrap : public JniPtrWrap<Neuro::DeviceScanner> {
public:
    JniDeviceScannerWrap(object_ptr_t deviceScannerPtr) : JniPtrWrap<Neuro::DeviceScanner>(deviceScannerPtr) {}

    void subscribeScanStateChanged(jobject scanStateChangedSubscriberRef);
    void subscribeDeviceFound(jobject deviceFoundSubscriberRef);

private:
    std::shared_ptr<jni::jobject_t> scanStateChangedGlobalSubscriberRef;
    Neuro::ListenerPtr<void, bool> mScanStateListener;

    std::shared_ptr<jni::jobject_t> deviceFoundGlobalSubscriberRef;
    Neuro::ListenerPtr<void, Neuro::DeviceSharedPtr> mDeviceFoundListener;
};

template<>
constexpr const char *jni::java_class_name<JniDeviceScannerWrap*>() { return "com/neuromd/neurosdk/DeviceScanner"; };

template<>
constexpr const char *jni::constructor_signature<JniDeviceScannerWrap*>() { return "(J)V"; };

#endif //ANDROID_JNI_NEURO_DEVICE_SCANNER_WRAP_H
