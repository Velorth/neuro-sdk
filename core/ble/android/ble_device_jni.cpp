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

#include <android/log.h>
#include "connection/ble_device_jni.h"

BleDeviceJni::BleDeviceJni(jobject bluetoothDevice, jobject context) : BleDevice() {
    __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "Constructor");
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    appContext = env->NewGlobalRef(context);

    //first we need to get name of bluetooth device
    auto btDeviceClass = env->GetObjectClass(bluetoothDevice);
    auto btDeviceNameMethod = env->GetMethodID(btDeviceClass, "getName", "()Ljava/lang/String;");
    auto btDeviceNameObj = (jstring) env->CallObjectMethod(bluetoothDevice, btDeviceNameMethod);

    //if name string is null it means that device is unavailable now
    if (btDeviceNameObj == NULL)
        throw std::runtime_error("Failed to initialize device");

    auto btDeviceName = env->GetStringUTFChars(btDeviceNameObj, NULL);
    deviceInfo = std::move(DeviceInfo::fromDeviceName(btDeviceName));
    env->ReleaseStringUTFChars(btDeviceNameObj, btDeviceName);

    //we need pass gattInfo to java BleDevice class constructor and we have java wrapper for
    //GattInfo class, so creating wrapper class from GattInfo class and passing it to java BleDevice
    //class constructor
    auto gattInfo = deviceInfo->getGattInfo();
    auto gattInfoWrapClass = env->FindClass("ru/neurotech/neurodevices/connection/DeviceGattInfo");
    auto gattInfoWrapConstructor = env->GetMethodID(gattInfoWrapClass, "<init>", "(J)V");
    auto gattInfoWrapper = env->NewObject(gattInfoWrapClass,
                                          gattInfoWrapConstructor,
                                          (jlong) gattInfo.get());

    auto bleDeviceClass = env->FindClass("ru/neurotech/neurodevices/connection/BleDevice");
    auto bleDeviceConstructor = env->GetMethodID(bleDeviceClass, "<init>",
                                                 "(Landroid/bluetooth/BluetoothDevice;Lru/neurotech/neurodevices/connection/DeviceGattInfo;)V");
    auto bleDevice = env->NewObject(bleDeviceClass, bleDeviceConstructor,
                                    bluetoothDevice, gattInfoWrapper);

    javaBleDevice = env->NewGlobalRef(bleDevice);

    //Instantiate callback class
    auto deviceCallbackWrapClass = env->FindClass("ru/neurotech/neurodevices/connection/BleDeviceCallback");
    auto deviceCallbackWrapConstructor = env->GetMethodID(deviceCallbackWrapClass, "<init>",
                                                          "(J)V");
    auto deviceCallbackWrapper = env->NewObject(deviceCallbackWrapClass,
                                                deviceCallbackWrapConstructor,
                                                (jlong) this);

    //And pass it to BleDevice
    auto subscribeMethod = env->GetMethodID(bleDeviceClass, "subscribeDeviceEvents",
                                            "(Lru/neurotech/neurodevices/connection/BleDeviceCallback;)V");
    env->CallVoidMethod(javaBleDevice, subscribeMethod, deviceCallbackWrapper);

    if (resCode == 1) jni::detach_thread();
}

BleDeviceJni::~BleDeviceJni() {
    __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "Destructor");
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    //Unsubscribe device events
    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto subscribeMethod = env->GetMethodID(bleDeviceClass, "subscribeDeviceEvents",
                                            "(Lru/neurotech/neurodevices/connection/BleDeviceCallback;)V");
    env->CallVoidMethod(javaBleDevice, subscribeMethod, NULL);

    env->DeleteGlobalRef(javaBleDevice);
    env->DeleteGlobalRef(appContext);

    if (resCode == 1) jni::detach_thread();
    __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "Destructor EXIT");
}

void BleDeviceJni::connect() {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto connectMethod = env->GetMethodID(bleDeviceClass, "connect",
                                          "(Landroid/content/Context;)V");
    env->CallVoidMethod(javaBleDevice, connectMethod, appContext);

    if (resCode == 1) jni::detach_thread();
}

void BleDeviceJni::disconnect() {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");
    __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "disconnect");

    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto disconnectMethod = env->GetMethodID(bleDeviceClass, "disconnect", "()V");
    env->CallVoidMethod(javaBleDevice, disconnectMethod);

    if (resCode == 1) jni::detach_thread();
}

void BleDeviceJni::close() {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");
    __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "close");

    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto closeMethod = env->GetMethodID(bleDeviceClass, "close", "()V");
    env->CallVoidMethod(javaBleDevice, closeMethod);

    if (resCode == 1) jni::detach_thread();
}

bool BleDeviceJni::sendCommand(const std::vector<Byte> &commandData) {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    jbyteArray retArray = env->NewByteArray((jsize) commandData.size());

    if (env->GetArrayLength(retArray) != commandData.size()) {
        env->DeleteLocalRef(retArray);
        retArray = env->NewByteArray((jsize) commandData.size());
    }

    void *tempCmdArray = env->GetPrimitiveArrayCritical((jarray) retArray, 0);
    memcpy(tempCmdArray, commandData.data(), commandData.size());
    env->ReleasePrimitiveArrayCritical(retArray, tempCmdArray, 0);

    __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "Sending command");
    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto sendCommandMethod = env->GetMethodID(bleDeviceClass, "sendCommand", "([B)Z");
    auto sendResult = env->CallBooleanMethod(javaBleDevice, sendCommandMethod, retArray);

    if (sendResult) __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "Command sent");
    else __android_log_print(ANDROID_LOG_VERBOSE, "BleDeviceJni", "Command not sent");

    if (resCode == 1) jni::detach_thread();
    return sendResult;
}

std::string BleDeviceJni::getName() const {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto getNameMethod = env->GetMethodID(bleDeviceClass, "name", "()Ljava/lang/String;");
    auto javaStringName = (jstring) env->CallObjectMethod(javaBleDevice, getNameMethod);


    std::string stringName;
    if (javaStringName != NULL){
        auto cName = env->GetStringUTFChars(javaStringName, NULL);
        stringName = cName;
        env->ReleaseStringUTFChars(javaStringName, cName);
    }
    else{
        stringName = "NULL";
        __android_log_print(ANDROID_LOG_WARN, "BleDeviceJni", "Name string is null");
    }

    if (resCode == 1) jni::detach_thread();
    return stringName;
}

std::string BleDeviceJni::getNetAddress() const {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto getAddressMethod = env->GetMethodID(bleDeviceClass, "address", "()Ljava/lang/String;");
    auto javaStringAddress = (jstring) env->CallObjectMethod(javaBleDevice, getAddressMethod);

    std::string stringAddress;
    if (javaStringAddress != NULL){
        auto cName = env->GetStringUTFChars(javaStringAddress, NULL);
        stringAddress = cName;
        env->ReleaseStringUTFChars(javaStringAddress, cName);
    }
    else{
        stringAddress = "NULL";
        __android_log_print(ANDROID_LOG_WARN, "BleDeviceJni", "Name string is null");
    }

    if (resCode == 1) jni::detach_thread();
    return stringAddress;
}

BleDeviceState BleDeviceJni::getState() const {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

    auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
    auto getStateMethod = env->GetMethodID(bleDeviceClass, "state", "()Lru/neurotech/neurodevices/connection/BleDeviceState;");
    auto javaState = env->CallObjectMethod(javaBleDevice, getStateMethod);

    BleDeviceState state;
    if (javaState != NULL){
        auto javaStateClass = env->FindClass("ru/neurotech/neurodevices/connection/DeviceGattInfo");
        auto javaStateGetCodeMethod = env->GetMethodID(javaStateClass, "getIntCode", "()I");
        auto stateCode = env->CallIntMethod(javaState, javaStateGetCodeMethod);
        state = parseBleDeviceState(stateCode);
    }
    else{
        state = BleDeviceState::ERROR;
        __android_log_print(ANDROID_LOG_ERROR, "BleDeviceJni", "State is null");
    }

    if (resCode == 1) jni::detach_thread();
    return state;
}

void BleDeviceJni::onConnected() {
    onDeviceStateChanged(BleDeviceState::CONNECTED, BleDeviceError::NO_ERROR);
}

void BleDeviceJni::onDisconnected() {
    onDeviceStateChanged(BleDeviceState::DISCONNECTED, BleDeviceError::NO_ERROR);
}

void BleDeviceJni::onError(BleDeviceError error) {
    onDeviceStateChanged(BleDeviceState::ERROR, error);
}

void BleDeviceJni::onDataReceived(const std::vector<Byte> &data) {
    BleDevice::onDataReceived(data);
}

void BleDeviceJni::onStatusReceived(const std::vector<Byte> &status) {
    BleDevice::onStatusReceived(status);
}


















