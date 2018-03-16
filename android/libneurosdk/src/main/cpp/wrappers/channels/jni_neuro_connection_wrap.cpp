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

#include "wrappers/jni_neuro_device_wrap.h"
#include "scanner_factory.h"
#include "wrappers/scan_state_callback.h"
#include "wrappers/jni_connection_callback.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurodevices_NeuroConnection_createNeuroConnectionObj(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobject appContext) {
    auto neuroConnectionPtr = getNeuroConnectionScanner(appContext).release();
    auto globalSubscriberRef = env->NewGlobalRef(
            instance);
    neuroConnectionPtr->subscribeScanStateChanged(
            [globalSubscriberRef](bool isScanning) {
                auto log = LoggerFactory::getCurrentPlatformLogger();
                log->debug("[%s] On scan state changed", __FUNCTION__);
                javaOnScanStateChanged(globalSubscriberRef, isScanning);
            });
    neuroConnectionPtr->subscribeDeviceFound(
            [globalSubscriberRef](std::shared_ptr<NeuroDevice> device) {
                //here we have to pass heap allocated shared_ptr to callback function
                //to be sure that shared_ptr and object, managed by it will be alive
                //as long as managed code needs
                auto log = LoggerFactory::getCurrentPlatformLogger();
                log->debug("[%s] On device found", __FUNCTION__);
                deviceFoundCallback<JniNeuroDevice>(globalSubscriberRef, device);
            });
    return reinterpret_cast<jlong>(neuroConnectionPtr);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroConnection_deleteNeuroConnectionObj(JNIEnv *env,
                                                                        jobject instance,
                                                                        jlong objPtr) {
    auto neuroConnection = (NeuroConnection *) objPtr;
    delete neuroConnection;
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroConnection_startScan__JI(JNIEnv *env, jobject instance,
                                                             jlong objPtr, jint timeout) {
    __android_log_print(ANDROID_LOG_VERBOSE, "NeuroConnectionWrap",
                        "Start scan. DeviceScanner ptr is %lld", objPtr);
    auto neuroConnection = (NeuroConnection *) objPtr;
    neuroConnection->startScan(timeout);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroConnection_stopScan__J(JNIEnv *env, jobject instance,
                                                           jlong objPtr) {
    auto neuroConnection = (NeuroConnection *) objPtr;
    neuroConnection->stopScan();
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurodevices_NeuroConnection_findDeviceByAddress(JNIEnv *env, jobject instance,
                                                                   jlong objPtr,
                                                                   jstring address_) {
    const char *address = env->GetStringUTFChars(address_, 0);

    auto neuroConnection = (NeuroConnection *) objPtr;

    auto devicePtr = neuroConnection->findDeviceByAddress(address);
    if (!devicePtr) return NULL;

    auto device = new JniNeuroDevice(devicePtr);

    //creating java wrapper object for Device
    jni::java_object<decltype(device)> deviceWrapObj(device);

    device->subscribeStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "deviceStateChanged"));
    device->subscribeBatteryLevelChanged(find_notifier<decltype(device)>(deviceWrapObj, "batteryLevelChanged"));

    env->ReleaseStringUTFChars(address_, address);

    return env->NewLocalRef(deviceWrapObj);
}

}