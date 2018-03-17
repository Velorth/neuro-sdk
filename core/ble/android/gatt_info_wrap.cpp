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

#include "java_environment.h"
#include "ble/ble_device_info.h"


extern "C"
{
/**
 * Get DeviceService UUID
 */
JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_connection_DeviceGattInfo_getDeviceServiceUUID(JNIEnv *env,
                                                                              jobject instance,
                                                                              jlong deviceInfoPtr) {
    auto gattInfo = (Neuro::DeviceGattInfo *) deviceInfoPtr;
    auto deviceServiceUUID = gattInfo->deviceServiceUUID();
    return env->NewStringUTF(deviceServiceUUID.c_str());
}

/**
 * Get receive characteristic UUID
 */
JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_connection_DeviceGattInfo_getRxCharacteristicUUID(JNIEnv *env,
                                                                                 jobject instance,
                                                                                 jlong deviceInfoPtr) {
    auto gattInfo = (Neuro::DeviceGattInfo *) deviceInfoPtr;
    auto rxCharUUID = gattInfo->rxCharacteristicUUID();
    return env->NewStringUTF(rxCharUUID.c_str());
}

/**
 * Get transmit characteristic UUID
 */
JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_connection_DeviceGattInfo_getTxCharacteristicUUID(JNIEnv *env,
                                                                                 jobject instance,
                                                                                 jlong deviceInfoPtr) {
    auto gattInfo = (Neuro::DeviceGattInfo *) deviceInfoPtr;
    auto txCharUUID = gattInfo->txCharacteristicUUID();
    return env->NewStringUTF(txCharUUID.c_str());
}

/**
 * Get status characteristic UUID
 */
JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_connection_DeviceGattInfo_getStatusCharacteristicUUID(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jlong deviceInfoPtr) {
    auto gattInfo = (Neuro::DeviceGattInfo *) deviceInfoPtr;
    auto statusCharUUID = gattInfo->statusCharacteristicUUID();
    return env->NewStringUTF(statusCharUUID.c_str());
}

/**
 * Get generic access service UUID
 */
JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_connection_DeviceGattInfo_getGenericAccessServiceUUID(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jlong deviceInfoPtr) {
    auto gattInfo = (Neuro::DeviceGattInfo *) deviceInfoPtr;
    auto genericServiceUUID = gattInfo->genericAccessUUID();
    return env->NewStringUTF(genericServiceUUID.c_str());
}

/**
 * Get CCCD
 */
JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_connection_DeviceGattInfo_getCCCD(JNIEnv *env, jobject instance,
                                                                 jlong deviceInfoPtr) {
    auto gattInfo = (Neuro::DeviceGattInfo *) deviceInfoPtr;
    auto cccd = gattInfo->cccd();
    return env->NewStringUTF(cccd.c_str());
}
}