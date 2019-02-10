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
#include "logger.h"

namespace Neuro {

std::string getBleName(jobject ble_device_obj){
    return jni::call_in_attached_thread([=](auto env) {
        //first we need to get name of bluetooth device
        auto btDeviceClass = env->GetObjectClass(ble_device_obj);
        auto btDeviceNameMethod = env->GetMethodID(btDeviceClass, "getName",
                                                   "()Ljava/lang/String;");
        auto btDeviceNameObj = (jstring) env->CallObjectMethod(ble_device_obj,
                                                               btDeviceNameMethod);

        //if name string is null it means that device is unavailable now
        if (btDeviceNameObj == NULL)
            throw std::runtime_error("Failed to initialize device");

        auto btNameChars = env->GetStringUTFChars(btDeviceNameObj, NULL);
        auto btDeviceName = std::string(btNameChars);

        env->ReleaseStringUTFChars(btDeviceNameObj, btNameChars);
        env->DeleteLocalRef(btDeviceNameObj);
        return btDeviceName;
    });
}

BleDeviceJni::BleDeviceJni(jobject bluetoothDevice, jobject context) :
    BleDevice(BleDeviceInfo::fromDeviceName(getBleName(bluetoothDevice))) {
    jni::call_in_attached_thread([=](auto env) {
        LOG_TRACE("Constructor");
        appContext = env->NewGlobalRef(context);

        javaStateClass = (jclass)env->NewGlobalRef(env->FindClass(
                "com/neuromd/bleconnection/device/BleDeviceState"));
        //we need pass gattInfo to java BleDevice class constructor and we have java wrapper for
        //GattInfo class, so creating wrapper class from GattInfo class and passing it to java BleDevice
        //class constructor
        auto gattInfo = this->getGattInfo()->getGattInfo();
        auto gattInfoWrapClass = env->FindClass(
                    "com/neuromd/bleconnection/device/DeviceGattInfo");
        auto gattInfoWrapConstructor = env->GetMethodID(gattInfoWrapClass, "<init>", "(J)V");
        auto gattInfoWrapper = env->NewObject(gattInfoWrapClass,
                                              gattInfoWrapConstructor,
                                              reinterpret_cast<jlong>(new std::shared_ptr<DeviceGattInfo>(gattInfo)));

        auto bleDeviceClass = env->FindClass("com/neuromd/bleconnection/device/BleDevice");
        auto bleDeviceConstructor = env->GetMethodID(bleDeviceClass, "<init>",
                                                     "(Landroid/bluetooth/BluetoothDevice;Lcom/neuromd/bleconnection/device/DeviceGattInfo;)V");
        auto bleDevice = env->NewObject(bleDeviceClass, bleDeviceConstructor,
                                        bluetoothDevice, gattInfoWrapper);

        javaBleDevice = env->NewGlobalRef(bleDevice);

        //Instantiate callback class
        auto deviceCallbackWrapClass = env->FindClass(
                    "com/neuromd/bleconnection/device/BleDeviceCallback");
        auto deviceCallbackWrapConstructor = env->GetMethodID(deviceCallbackWrapClass, "<init>",
                                                              "(J)V");
        auto deviceCallbackWrapper = env->NewObject(deviceCallbackWrapClass,
                                                    deviceCallbackWrapConstructor,
                                                    (jlong) this);

        //And pass it to BleDevice
        auto subscribeMethod = env->GetMethodID(bleDeviceClass, "subscribeDeviceEvents",
                                                "(Lcom/neuromd/bleconnection/device/BleDeviceCallback;)V");
        env->CallVoidMethod(javaBleDevice, subscribeMethod, deviceCallbackWrapper);
    });
}

BleDeviceJni::~BleDeviceJni() {
    try {
        jni::call_in_attached_thread([=](auto env) {
            LOG_TRACE("Destructor");
            auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
            auto subscribeMethod = env->GetMethodID(bleDeviceClass, "subscribeDeviceEvents",
                                                    "(Lcom/neuromd/bleconnection/device/BleDeviceCallback;)V");
            env->CallVoidMethod(javaBleDevice, subscribeMethod, NULL);
            env->DeleteGlobalRef(javaStateClass);
            env->DeleteGlobalRef(javaBleDevice);
            env->DeleteGlobalRef(appContext);
            LOG_TRACE("Destructor EXIT");
        });
    }
    catch (std::runtime_error &e){
        LOG_ERROR_V("Destructon failure: %s", e.what());
    }
}

void BleDeviceJni::connect() {
    jni::call_in_attached_thread([=](auto env) {
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto connectMethod = env->GetMethodID(bleDeviceClass, "connect",
                                              "(Landroid/content/Context;)V");
        env->CallVoidMethod(javaBleDevice, connectMethod, appContext);
    });
}

void BleDeviceJni::disconnect() {
    jni::call_in_attached_thread([=](auto env) {
        LOG_TRACE("disconnect");
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto disconnectMethod = env->GetMethodID(bleDeviceClass, "disconnect", "()V");
        env->CallVoidMethod(javaBleDevice, disconnectMethod);
    });
}

void BleDeviceJni::close() {
    jni::call_in_attached_thread([=](auto env) {
        LOG_TRACE("close");
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto closeMethod = env->GetMethodID(bleDeviceClass, "close", "()V");
        env->CallVoidMethod(javaBleDevice, closeMethod);
    });
}

bool BleDeviceJni::sendCommand(const std::vector<Byte> &commandData) {
    return jni::call_in_attached_thread([=](auto env) {
        jbyteArray retArray = env->NewByteArray((jsize) commandData.size());

        if (env->GetArrayLength(retArray) != commandData.size()) {
            env->DeleteLocalRef(retArray);
            retArray = env->NewByteArray((jsize) commandData.size());
        }

        void *tempCmdArray = env->GetPrimitiveArrayCritical((jarray) retArray, 0);
        memcpy(tempCmdArray, commandData.data(), commandData.size());
        env->ReleasePrimitiveArrayCritical(retArray, tempCmdArray, 0);

        LOG_TRACE("Sending command");
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto sendCommandMethod = env->GetMethodID(bleDeviceClass, "sendCommand", "([B)Z");
        auto sendResult = env->CallBooleanMethod(javaBleDevice, sendCommandMethod, retArray);

        if (sendResult) {
            LOG_TRACE("Command sent");
        }
        else {
            LOG_TRACE("Command not sent");
        }

        return sendResult;
    });
}

std::string BleDeviceJni::getName() const {
    return jni::call_in_attached_thread([=](auto env) {
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto getNameMethod = env->GetMethodID(bleDeviceClass, "name", "()Ljava/lang/String;");
        auto javaStringName = (jstring) env->CallObjectMethod(javaBleDevice, getNameMethod);


        std::string stringName;
        if (javaStringName != NULL) {
            auto cName = env->GetStringUTFChars(javaStringName, NULL);
            stringName = cName;
            env->ReleaseStringUTFChars(javaStringName, cName);
            env->DeleteLocalRef(javaStringName);
        } else {
            stringName = "NULL";
            LOG_WARN("Name string is null");
        }

        return stringName;
    });
}

std::string BleDeviceJni::getNetAddress() const {
    return jni::call_in_attached_thread([=](auto env) {
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto getAddressMethod = env->GetMethodID(bleDeviceClass, "address",
                                                 "()Ljava/lang/String;");
        auto javaStringAddress = (jstring) env->CallObjectMethod(javaBleDevice,
                                                                 getAddressMethod);

        std::string stringAddress;
        if (javaStringAddress != NULL) {
            auto cName = env->GetStringUTFChars(javaStringAddress, NULL);
            stringAddress = cName;
            env->ReleaseStringUTFChars(javaStringAddress, cName);
            env->DeleteLocalRef(javaStringAddress);
        } else {
            stringAddress = "NULL";
            LOG_WARN("Name string is null");
        }

        return stringAddress;
    });
}

BleDeviceState BleDeviceJni::getState() const {
    return jni::call_in_attached_thread([=](auto env) {
        auto bleDeviceClass = env->GetObjectClass(javaBleDevice);
        auto getStateMethod = env->GetMethodID(bleDeviceClass, "state",
                                               "()Lcom/neuromd/bleconnection/device/BleDeviceState;");
        auto javaState = env->CallObjectMethod(javaBleDevice, getStateMethod);

        BleDeviceState state;
        if (javaState != NULL) {
            auto javaStateGetCodeMethod = env->GetMethodID(javaStateClass, "getIntCode", "()I");
            auto stateCode = env->CallIntMethod(javaState, javaStateGetCodeMethod);
            state = parseBleDeviceState(stateCode);
            env->DeleteLocalRef(javaState);
        } else {
            state = BleDeviceState::Error;
            LOG_WARN("State is null");
        }

        return state;
    });
}

void BleDeviceJni::onConnected() {
    notifyStateChanged(BleDeviceState::Connected, BleDeviceError::NoError);
}

void BleDeviceJni::onDisconnected() {
    notifyStateChanged(BleDeviceState::Disconnected, BleDeviceError::NoError);
}

void BleDeviceJni::onError(BleDeviceError error) {
    notifyStateChanged(BleDeviceState::Error, error);
}

void BleDeviceJni::onDataReceived(const std::vector<Byte> &data) {
    notifyDataReceived(data);
}

void BleDeviceJni::onStatusReceived(const std::vector<Byte> &status) {
    notifyStatusReceived(status);
}

}
















