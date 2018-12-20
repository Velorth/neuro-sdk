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

#include "logger.h"
#include "ble/android/bluetooth_scanner.h"
#include "ble/android/ble_device_jni.h"

namespace Neuro {
    BluetoothScannerJni::BluetoothScannerJni(jobject context) {
        LOG_TRACE("Constructor");
        isScanProcessing.store(false);
        jni::call_in_attached_thread([=](auto env) {
            appContext = env->NewGlobalRef(context);
            auto scannerFactoryClass = env->FindClass(
                    "com/neuromd/bleconnection/scanner/BleDeviceScannerFactory");
            auto factoryMethod = env->GetStaticMethodID(scannerFactoryClass, "getBleDeviceScanner",
                                                        "()Lcom/neuromd/bleconnection/scanner/BleDeviceScanner;");
            auto scannerObj = env->CallStaticObjectMethod(scannerFactoryClass, factoryMethod);
            javaScannerInstance = env->NewGlobalRef(scannerObj);
        });
    }

    BluetoothScannerJni::~BluetoothScannerJni() {
        try {
            jni::call_in_attached_thread([=](auto env) {
                env->DeleteGlobalRef(javaScannerInstance);
                env->DeleteGlobalRef(appContext);
            });
        }
        catch (std::runtime_error &e){
            LOG_ERROR_V("Error during destruction: %s", e.what());
        }
    }

    void BluetoothScannerJni::startScan() {
        emulator.startScan();
        jni::call_in_attached_thread([=](auto env) {
            LOG_TRACE("Start scan");
            auto scannerClass = env->GetObjectClass(javaScannerInstance);
            auto resetMethodID = env->GetMethodID(scannerClass, "reset", "()V");
            env->CallVoidMethod(javaScannerInstance, resetMethodID);
            auto scanMethodID = env->GetMethodID(scannerClass, "startScan",
                                                 "(Landroid/content/Context;)V");
            env->CallVoidMethod(javaScannerInstance, scanMethodID, appContext);
        });
        isScanProcessing.store(true);
    }

    void BluetoothScannerJni::stopScan() {
        emulator.stopScan();
        jni::call_in_attached_thread([=](auto env) {
            LOG_TRACE("Stop scan");
            auto scannerClass = env->GetObjectClass(javaScannerInstance);
            auto stopMethodID = env->GetMethodID(scannerClass, "stopScan", "()V");
            env->CallVoidMethod(javaScannerInstance, stopMethodID);
        });
        isScanProcessing.store(false);
    }

    std::unique_ptr<BleDevice> BluetoothScannerJni::getDeviceByAddress(std::string address) {

        auto emulatedDevice = emulator.getDeviceByAddress(address);
        if (emulatedDevice)
            return emulatedDevice;

        return jni::call_in_attached_thread([=](auto env) {
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
                LOG_ERROR_V("Failed get device by address: %s", error.what());
            }

            env->DeleteLocalRef(bluetoothDevice);

            return bleDevice;
        });
    }

    void BluetoothScannerJni::setFilter(std::vector<std::string> filterList) {
        emulator.setFilter(filterList);
        jni::call_in_attached_thread([=](auto env) {
            //Creating filter object
            auto filterClass = env->FindClass("com/neuromd/bleconnection/device/DeviceFilter");
            auto filterConstructor = env->GetMethodID(filterClass, "<init>", "()V");
            auto filterObject = env->NewObject(filterClass, filterConstructor);
            auto addFilterMethod = env->GetMethodID(filterClass, "addFilter",
                                                    "(Ljava/lang/String;)V");

            //Filling it with supported device names
            for (auto filter: filterList) {
                auto javaFilter = env->NewStringUTF(filter.c_str());
                env->CallVoidMethod(filterObject, addFilterMethod, javaFilter);
            }

            //Add filter object to scanner
            auto scannerClass = env->GetObjectClass(javaScannerInstance);
            auto setFilterMethodID = env->GetMethodID(scannerClass, "setFilter",
                                                      "(Lcom/neuromd/bleconnection/device/DeviceFilter;)V");
            env->CallVoidMethod(javaScannerInstance, setFilterMethodID, filterObject);
        });
    }

    void BluetoothScannerJni::subscribeDeviceFound(
            std::function<void(std::unique_ptr<BleDevice>)> callbackFunction){
        emulator.subscribeDeviceFound(callbackFunction);
        deviceFoundCallback = callbackFunction;

        jni::call_in_attached_thread([=](auto env) {
            //create java callback class
            auto deviceFoundCallbackClass = env->FindClass(
                    "com/neuromd/bleconnection/device/DeviceFoundCallback");
            auto deviceFoundCallbackCtor = env->GetMethodID(deviceFoundCallbackClass, "<init>",
                                                            "(J)V");

            LOG_TRACE_V("Subscribing device found, callback ptr is %ld", reinterpret_cast<long>(this));
            auto deviceFoundCallbackObj = env->NewObject(deviceFoundCallbackClass,
                                                         deviceFoundCallbackCtor,
                                                         (jlong) this);

            //pass callback class to scanner
            auto scannerClass = env->GetObjectClass(javaScannerInstance);
            auto setCallbackMethodID = env->GetMethodID(scannerClass, "subscribeDeviceFound",
                                                        "(Lcom/neuromd/bleconnection/device/DeviceFoundCallback;)V");
            env->CallVoidMethod(javaScannerInstance, setCallbackMethodID, deviceFoundCallbackObj);
        });
    }

    void BluetoothScannerJni::onDeviceFound(jobject bluetoothDevice) {
        LOG_TRACE("On device found function");
        std::unique_ptr<BleDeviceJni> bleDevice(new BleDeviceJni(bluetoothDevice, appContext));
        try {
        if (deviceFoundCallback) deviceFoundCallback(std::move(bleDevice));
        }
        catch (std::exception &e){
            LOG_ERROR_V("Exception in callback function: %s", e.what());
        }
        catch (...){
            LOG_ERROR("Unhandled exception in callback function");
        }
    }

    void BluetoothScannerJni::releaseDevice(std::string name, std::string address) {

        emulator.releaseDevice(name, address);

        jni::call_in_attached_thread([=](auto env) {
            auto deviceName = env->NewStringUTF(name.c_str());
            auto deviceAddress = env->NewStringUTF(address.c_str());

            auto scannerClass = env->GetObjectClass(javaScannerInstance);
            auto releaseDeviceMethodID = env->GetMethodID(scannerClass, "releaseDevice",
                                                          "(Ljava/lang/String;Ljava/lang/String;)V");
            env->CallVoidMethod(javaScannerInstance, releaseDeviceMethodID, deviceName,
                                deviceAddress);
        });
    }
}















