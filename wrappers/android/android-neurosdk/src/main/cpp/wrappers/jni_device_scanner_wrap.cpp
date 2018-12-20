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


#include <android/log.h>
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/jni_device_scanner_wrap.h"
#include "logger.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_create(JNIEnv *env, jobject instance, jobject appContext) {
    try {
        auto scanner = std::shared_ptr<Neuro::DeviceScanner>(Neuro::createDeviceScanner(appContext));
        auto channelWrap = new JniDeviceScannerWrap(scanner);
        return reinterpret_cast<jlong>(channelWrap);
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_ERROR, "CreateDeviceScanner",
                            "Error creating scanner: %s", e.what());
        jni::java_throw(env, "java/lang/IllegalArgumentException", e);
        return 0;
    }
}

JNIEXPORT void
JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_init(JNIEnv *env, jobject instance) {

    auto deviceScannerWrap = extract_pointer<JniDeviceScannerWrap>(env, instance);
    deviceScannerWrap->subscribeScanStateChanged(
            find_notifier<decltype(deviceScannerWrap)>(instance, "scanStateChanged"));
    deviceScannerWrap->subscribeDeviceFound(
            find_notifier<decltype(deviceScannerWrap)>(instance, "deviceFound"));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniDeviceScannerWrap>(env, instance);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_startScan(JNIEnv *env, jobject instance, jint timeout) {
    auto &deviceScannerWrap = *extract_pointer<JniDeviceScannerWrap>(env, instance);
    deviceScannerWrap->startScan(timeout);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_stopScan(JNIEnv *env, jobject instance) {
    auto &deviceScannerWrap = *extract_pointer<JniDeviceScannerWrap>(env, instance);
    deviceScannerWrap->stopScan();
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_findDeviceByAddress(JNIEnv *env, jobject instance, jstring address_) {
    const char *address = env->GetStringUTFChars(address_, 0);

    auto &deviceScannerWrap = *extract_pointer<JniDeviceScannerWrap>(env, instance);

    auto devicePtr = deviceScannerWrap->findDeviceByAddress(address);
    if (!devicePtr) return NULL;

    auto device = new JniDeviceWrap(std::move(devicePtr));
    jni::java_object<decltype(device)> deviceWrapObj(device);
    env->ReleaseStringUTFChars(address_, address);

    return deviceWrapObj;
}

}

void JniDeviceScannerWrap::subscribeScanStateChanged(jobject stateChangedSubscriberRef) {
    scanStateChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = scanStateChangedGlobalSubscriberRef;
    mScanStateListener = this->object->subscribeScanStateChanged([weakReference](auto isScanning){
        jni::call_in_attached_thread([&weakReference, &isScanning](auto env){
            sendNotification<bool>(env, weakReference, isScanning);
        });
    });
}

void JniDeviceScannerWrap::subscribeDeviceFound(jobject deviceFoundSubscriberRef) {
    deviceFoundGlobalSubscriberRef = jni::make_global_ref_ptr(deviceFoundSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = deviceFoundGlobalSubscriberRef;
    mDeviceFoundListener = this->object->subscribeDeviceFound([weakReference](auto devicePtr){
        jni::call_in_attached_thread([&weakReference, &devicePtr](auto env){
            auto subscriberRef = weakReference.lock();
            if (!subscriberRef)
                return;

            auto device = new JniDeviceWrap(std::move(devicePtr));
            jni::java_object<decltype(device)> deviceWrapObj(device);
            callJavaSendNotification(env, subscriberRef.get(), static_cast<jobject>(deviceWrapObj));
            env->DeleteLocalRef(deviceWrapObj);
        });
    });
}