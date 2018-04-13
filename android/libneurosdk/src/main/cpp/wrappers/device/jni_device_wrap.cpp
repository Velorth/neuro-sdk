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

#include "wrappers/jni_device_wrap.h"
#include "java_environment.h"

extern "C"
{

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_deleteDevice(JNIEnv *env, jobject instance,
                                                        jlong objPtr) {
    auto devicePtr = reinterpret_cast<JniDeviceWrap *>(objPtr);
    delete devicePtr;
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_connectDevice(JNIEnv *env, jobject instance,
                                                         jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    devicePtr->connect();
}


JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_disconnectFromDevice(JNIEnv *env, jobject instance,
                                                                jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    devicePtr->disconnect();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_closeDevice(JNIEnv *env, jobject instance,
                                                       jlong objPtr) {

    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    devicePtr->close();
}

JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_getDeviceName(JNIEnv *env, jobject instance,
                                                         jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    auto name = devicePtr->getName();
    return env->NewStringUTF(name.c_str());
}

JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_getDeviceAddress(JNIEnv *env, jobject instance,
                                                            jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    auto address = devicePtr->getAddress();
    return env->NewStringUTF(address.c_str());
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_getBatteryLevel__J(JNIEnv *env, jobject instance,
                                                              jlong objPtr) {

    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    return devicePtr->getBatteryLevel();
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_getDeviceState(JNIEnv *env, jobject instance,
                                                          jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    auto state = devicePtr->getState();
    LoggerFactory::getCurrentPlatformLogger()->debug("[NeuroDeviceWrap: getDeviceState] State: %d",
                                                     static_cast<int>(state));
    return env->NewLocalRef(jni::java_object<DeviceState>(state));
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_getDeviceError(JNIEnv *env, jobject instance,
                                                          jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    //TODO return device error
    return NULL;
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurodevices_NeuroDevice_getFeatures(JNIEnv *env, jobject instance,
                                                       jlong objPtr) {
    auto& devicePtr = *reinterpret_cast<JniDeviceWrap *>(objPtr);
    auto features = devicePtr->getFeatures();
    auto featureEnum = env->FindClass("ru/neurotech/neurodevices/DeviceFeature");
    auto getFeatureMethod = env->GetStaticMethodID(featureEnum, "fromIntCode",
                                                   "(I)Lru/neurotech/neurodevices/DeviceFeature;");

    jobjectArray featuresArray = env->NewObjectArray(features.size(), featureEnum, NULL);

    for (auto it = features.begin(); it != features.end(); ++it) {
        auto intFeature = static_cast<int>(*it);
        auto featureObj = env->CallStaticObjectMethod(featureEnum, getFeatureMethod, intFeature);
        env->SetObjectArrayElement(featuresArray, it - features.begin(), featureObj);
    }

    return featuresArray;
}

}

