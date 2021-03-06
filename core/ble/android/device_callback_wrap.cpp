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

#include "ble/android/ble_device_jni.h"

extern "C"
{
JNIEXPORT void JNICALL
Java_com_neuromd_bleconnection_device_BleDeviceCallback_onConnected__J(JNIEnv *env, jobject instance,
                                                                  jlong callbackPtr) {
    auto bleDevice = (Neuro::BleDeviceJni *) callbackPtr;
    bleDevice->onConnected();
}

JNIEXPORT void JNICALL
Java_com_neuromd_bleconnection_device_BleDeviceCallback_onDisconnected__J(JNIEnv *env, jobject instance,
                                                                  jlong callbackPtr) {
    auto bleDevice = (Neuro::BleDeviceJni *) callbackPtr;
    bleDevice->onDisconnected();
}

JNIEXPORT void JNICALL
Java_com_neuromd_bleconnection_device_BleDeviceCallback_onBluetoothDeviceError__JI(JNIEnv *env,
                                                                          jobject instance,
                                                                          jlong callbackPtr,
                                                                          jint error) {
    auto bleDevice = (Neuro::BleDeviceJni *) callbackPtr;
    bleDevice->onError(Neuro::parseBleErrorType(error));
}

JNIEXPORT void JNICALL
Java_com_neuromd_bleconnection_device_BleDeviceCallback_onDataReceived(JNIEnv *env, jobject instance,
                                                                  jlong callbackPtr,
                                                                  jbyteArray data_, jint length) {
    if (length <= 0) return;
    jbyte *data = env->GetByteArrayElements(data_, NULL);
    std::vector<Neuro::Byte> receivedData(data, data+length);

    auto bleDevice = (Neuro::BleDeviceJni *) callbackPtr;
    bleDevice->onDataReceived(receivedData);

    env->ReleaseByteArrayElements(data_, data, 0);
}

JNIEXPORT void JNICALL
Java_com_neuromd_bleconnection_device_BleDeviceCallback_onStatusReceived(JNIEnv *env, jobject instance,
                                                                    jlong callbackPtr,
                                                                    jbyteArray statusData_,
                                                                    jint length) {
    if (length <= 0) return;
    jbyte *statusData = env->GetByteArrayElements(statusData_, NULL);
    std::vector<Neuro::Byte> receivedStatus(statusData, statusData+length);

    auto bleDevice = (Neuro::BleDeviceJni *) callbackPtr;
    bleDevice->onStatusReceived(receivedStatus);
    env->ReleaseByteArrayElements(statusData_, statusData, 0);
}
}