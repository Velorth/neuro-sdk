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

#include "java_helper.h"
#include "clistener.h"
#include "cscanner.h"
#include "sdk_error.h"

void onDeviceFound(DeviceScanner *scanner, Device *device, void *listener_helper){
    auto listenerHelper = reinterpret_cast<ListenerHelper *>(listener_helper);
    listenerHelper->notify(reinterpret_cast<jlong>(scanner), reinterpret_cast<jlong>(device));
}

void onScanStateChanged(DeviceScanner *scanner, bool is_scanning, void *listener_helper){
    auto listenerHelper = reinterpret_cast<ListenerHelper *>(listener_helper);
    listenerHelper->notify(reinterpret_cast<jlong>(scanner), static_cast<jboolean>(is_scanning));
}

extern "C"
{

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_freeListenerHandle(JNIEnv *, jclass, jlong listenerHandle) {

   auto listenerHelperPtr = reinterpret_cast<ListenerHelper *>(listenerHandle);
   delete listenerHelperPtr;
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_createDeviceScanner(JNIEnv *env, jclass, jobject context) {

    auto scannerPtr = create_device_scanner(context);
    if (scannerPtr == nullptr){
        char message[256];
        sdk_last_error_msg(message, 256);
        java_throw(env, message);
        return nullptr;
    }

    return reinterpret_cast<jlong>(scannerPtr);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerDelete(JNIEnv *, jclass, jlong scannerPtr) {

    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    scanner_delete(scanner);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerStartScan(JNIEnv *env, jclass, jlong scannerPtr, jint timeoutMs) {

    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    throw_if_error(env, scanner_start_scan(scanner, timeoutMs));
}


JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerStopScan(JNIEnv *env, jclass, jlong scannerPtr) {

    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    throw_if_error(env, scanner_stop_scan(scanner));
}

JNIEXPORT jboolean JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerIsScanning(JNIEnv *env, jclass type,
                                                          jlong scannerPtr) {

    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    bool isScanning;
    throw_if_error(env, scanner_is_scanning(scanner, &isScanning));
    return static_cast<jboolean>(isScanning);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerSubscribeDeviceFound(JNIEnv *env, jobject instance, jlong scannerPtr) {

    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    auto helperPtr = make_listener_helper(env, instance, "onDeviceFound", "(JJ)V");
    throw_if_error(env, scanner_set_device_found_callback(scanner, &onDeviceFound, helperPtr->listnerHandlePointer(), helperPtr));
    return reinterpret_cast<jlong>(helperPtr);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerSubscribeScanState(JNIEnv *env, jobject instance, jlong scannerPtr) {

    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    auto helperPtr = make_listener_helper(env, instance, "onScanStateChanged", "(JZ)V");
    throw_if_error(env, scanner_set_scan_state_callback(scanner, &onScanStateChanged, helperPtr->listnerHandlePointer(), helperPtr));
    return reinterpret_cast<jlong>(helperPtr);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_DeviceScanner_scannerGetDeviceByAddress(JNIEnv *env, jclass,
                                                                  jlong scannerPtr,
                                                                  jstring addressString) {
    auto scanner = reinterpret_cast<DeviceScanner *>(scannerPtr);
    const char *address = env->GetStringUTFChars(addressString, 0);
    auto device = scanner_get_device_by_address(scanner, address);
    env->ReleaseStringUTFChars(addressString, address);
    if (device == nullptr){
        char message[256];
        sdk_last_error_msg(message, 256);
        java_throw(env, message);
        return nullptr;
    }
    return reinterpret_cast<jlong>(device);
}

}
