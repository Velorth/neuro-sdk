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
#include "ble/android/bluetooth_scanner.h"
#include "ble/android/ble_device_jni.h"

namespace Neuro {
    BluetoothScannerJni::BluetoothScannerJni(jobject context) {
        __android_log_print(ANDROID_LOG_VERBOSE, "BluetoothScannerJni", "Constructor");
        isScanProcessing.store(false);
        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        appContext = env->NewGlobalRef(context);

        auto scannerFactoryClass = env->FindClass(
                "ru/neurotech/neurodevices/scanner/BleDeviceScannerFactory");
        auto factoryMethod = env->GetStaticMethodID(scannerFactoryClass, "getBleDeviceScanner",
                                                    "()Lru/neurotech/neurodevices/scanner/BleDeviceScanner;");
        auto scannerObj = env->CallStaticObjectMethod(scannerFactoryClass, factoryMethod);

        javaScannerInstance = env->NewGlobalRef(scannerObj);

        if (resCode == 1) jni::detach_thread();
    }

    BluetoothScannerJni::~BluetoothScannerJni() {
        __android_log_print(ANDROID_LOG_VERBOSE, "BluetoothScannerJni", "Destructor");

        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        env->DeleteGlobalRef(javaScannerInstance);
        env->DeleteGlobalRef(appContext);

        if (resCode == 1) jni::detach_thread();
    }

    void BluetoothScannerJni::startScan() {

        emulator.startScan();

        __android_log_print(ANDROID_LOG_VERBOSE, "BluetoothScannerJni", "Start scan");
        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        auto scannerClass = env->GetObjectClass(javaScannerInstance);
        auto resetMethodID = env->GetMethodID(scannerClass, "reset", "()V");
        env->CallVoidMethod(javaScannerInstance, resetMethodID);

        auto scanMethodID = env->GetMethodID(scannerClass, "startScan",
                                             "(Landroid/content/Context;)V");
        env->CallVoidMethod(javaScannerInstance, scanMethodID, appContext);

        if (resCode == 1) jni::detach_thread();
        isScanProcessing.store(true);
    }

    void BluetoothScannerJni::stopScan() {

        emulator.stopScan();

        __android_log_print(ANDROID_LOG_VERBOSE, "BluetoothScannerJni", "Stop scan");
        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        auto scannerClass = env->GetObjectClass(javaScannerInstance);
        auto stopMethodID = env->GetMethodID(scannerClass, "stopScan", "()V");
        env->CallVoidMethod(javaScannerInstance, stopMethodID);

        if (resCode == 1) jni::detach_thread();
        isScanProcessing.store(false);
    }

    std::unique_ptr<BleDevice> BluetoothScannerJni::getDeviceByAddress(std::string address) {

        auto emulatedDevice = emulator.getDeviceByAddress(address);
        if (emulatedDevice)
            return emulatedDevice;

        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        auto scannerClass = env->GetObjectClass(javaScannerInstance);
        auto getDeviceMethodID = env->GetMethodID(scannerClass, "getDeviceByAddress",
                                                  "(Landroid/content/Context;Ljava/lang/String;)Landroid/bluetooth/BluetoothDevice;");
        auto addressJavaString = env->NewStringUTF(address.c_str());
        auto bluetoothDevice = env->CallObjectMethod(javaScannerInstance, getDeviceMethodID,
                                                     appContext, addressJavaString);

        if (bluetoothDevice == NULL)
            return std::unique_ptr<BleDevice>();

        std::unique_ptr<BleDevice> bleDevice;

        try {
            bleDevice.reset(new BleDeviceJni(bluetoothDevice, appContext));
        }
        catch (std::runtime_error &error) {
            __android_log_print(ANDROID_LOG_ERROR, "BluetoothScannerJni",
                                "Failed get device by address: %s", error.what());
        }

        if (resCode == 1) jni::detach_thread();

        return bleDevice;
    }

    void BluetoothScannerJni::setFilter(std::vector<std::string> filterList) {

        emulator.setFilter(filterList);

        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        //Creating filter object
        auto filterClass = env->FindClass("ru/neurotech/neurodevices/connection/DeviceFilter");
        auto filterConstructor = env->GetMethodID(filterClass, "<init>", "()V");
        auto filterObject = env->NewObject(filterClass, filterConstructor);
        auto addFilterMethod = env->GetMethodID(filterClass, "addFilter", "(Ljava/lang/String;)V");

        //Filling it with supported device names
        for (auto filter: filterList) {
            auto javaFilter = env->NewStringUTF(filter.c_str());
            env->CallVoidMethod(filterObject, addFilterMethod, javaFilter);
        }

        //Add filter object to scanner
        auto scannerClass = env->GetObjectClass(javaScannerInstance);
        auto setFilterMethodID = env->GetMethodID(scannerClass, "setFilter",
                                                  "(Lru/neurotech/neurodevices/connection/DeviceFilter;)V");
        env->CallVoidMethod(javaScannerInstance, setFilterMethodID, filterObject);

        if (resCode == 1) jni::detach_thread();
    }

    void BluetoothScannerJni::subscribeDeviceFound(
            std::function<void(std::unique_ptr<BleDevice>)> callbackFunction) {

        emulator.subscribeDeviceFound(callbackFunction);

        deviceFoundCallback = callbackFunction;

        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");

        //create java callback class
        auto deviceFoundCallbackClass = env->FindClass(
                "ru/neurotech/neurodevices/connection/DeviceFoundCallback");
        auto deviceFoundCallbackCtor = env->GetMethodID(deviceFoundCallbackClass, "<init>", "(J)V");

        __android_log_print(ANDROID_LOG_VERBOSE, "BluetoothScannerJni",
                            "Subscribing device found, callback ptr is %lld", (jlong) this);
        auto deviceFoundCallbackObj = env->NewObject(deviceFoundCallbackClass,
                                                     deviceFoundCallbackCtor,
                                                     (jlong) this);

        //pass callback class to scanner
        auto scannerClass = env->GetObjectClass(javaScannerInstance);
        auto setCallbackMethodID = env->GetMethodID(scannerClass, "subscribeDeviceFound",
                                                    "(Lru/neurotech/neurodevices/connection/DeviceFoundCallback;)V");
        env->CallVoidMethod(javaScannerInstance, setCallbackMethodID, deviceFoundCallbackObj);

        if (resCode == 1) jni::detach_thread();
    }

    void BluetoothScannerJni::onDeviceFound(jobject bluetoothDevice) {
        __android_log_print(ANDROID_LOG_VERBOSE, "BluetoothScannerJni", "On device found function");
        std::unique_ptr<BleDeviceJni> bleDevice(new BleDeviceJni(bluetoothDevice, appContext));
        if (deviceFoundCallback) deviceFoundCallback(std::move(bleDevice));

    }

    void BluetoothScannerJni::releaseDevice(std::string name, std::string address) {

        emulator.releaseDevice(name, address);

        JNIEnv *env;
        auto resCode = jni::get_env(&env);
        if (resCode == 2) throw std::runtime_error("Cannot retrieve JNI environment");


        auto deviceName = env->NewStringUTF(name.c_str());
        auto deviceAddress = env->NewStringUTF(address.c_str());

        auto scannerClass = env->GetObjectClass(javaScannerInstance);
        auto releaseDeviceMethodID = env->GetMethodID(scannerClass, "releaseDevice",
                                                      "(Ljava/lang/String;Ljava/lang/String;)V");
        env->CallVoidMethod(javaScannerInstance, releaseDeviceMethodID, deviceName, deviceAddress);

        if (resCode == 1) jni::detach_thread();
    }

}















