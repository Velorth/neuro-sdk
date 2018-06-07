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


#include "wrappers/device/jni_device_wrap.h"
#include "device_scanner.h"
#include "wrappers/scan_state_callback.h"
#include "wrappers/jni_connection_callback.h"
#include "logger.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_create(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobject appContext) {
    auto neuroConnectionPtr = Neuro::createDeviceScanner(appContext).release();
    auto globalSubscriberRef = env->NewGlobalRef(
            instance);
    neuroConnectionPtr->subscribeScanStateChanged(
            [globalSubscriberRef](bool isScanning) {
                auto log = LoggerFactory::getCurrentPlatformLogger();
                log->debug("[%s] On scan state changed", __FUNCTION__);
                javaOnScanStateChanged(globalSubscriberRef, isScanning);
            });
    neuroConnectionPtr->subscribeDeviceFound(
            [globalSubscriberRef](std::unique_ptr<Neuro::Device> device) {
                //here we have to pass heap allocated shared_ptr to callback function
                //to be sure that shared_ptr and object, managed by it will be alive
                //as long as managed code needs
                auto log = LoggerFactory::getCurrentPlatformLogger();
                log->debug("[%s] On device found", __FUNCTION__);
                deviceFoundCallback<JniDeviceWrap>(globalSubscriberRef, std::move(device));
            });
    return reinterpret_cast<jlong>(neuroConnectionPtr);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_deleteNative(JNIEnv *env,
                                                                        jobject instance,
                                                                        jlong objPtr) {
    auto neuroConnection = reinterpret_cast<Neuro::DeviceScanner *>(objPtr);
    delete neuroConnection;
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_startScan__JI(JNIEnv *env, jobject instance,
                                                             jlong objPtr, jint timeout) {
    auto neuroConnection = reinterpret_cast<Neuro::DeviceScanner *>(objPtr);
    neuroConnection->startScan(timeout);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_stopScan__J(JNIEnv *env, jobject instance,
                                                           jlong objPtr) {
    auto neuroConnection = reinterpret_cast<Neuro::DeviceScanner *>(objPtr);
    neuroConnection->stopScan();
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_findDeviceByAddress(JNIEnv *env, jobject instance,
                                                                   jlong objPtr,
                                                                   jstring address_) {
    const char *address = env->GetStringUTFChars(address_, 0);

    auto neuroConnection = reinterpret_cast<Neuro::DeviceScanner *>(objPtr);

    auto devicePtr = neuroConnection->findDeviceByAddress(address);
    if (!devicePtr) return NULL;

    auto device = new JniDeviceWrap(std::move(devicePtr));
    jni::java_object<decltype(device)> deviceWrapObj(device);
    env->ReleaseStringUTFChars(address_, address);

    return deviceWrapObj;
}

}