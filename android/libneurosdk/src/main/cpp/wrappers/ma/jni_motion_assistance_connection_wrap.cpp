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

#include "wrappers/jni_connection_callback.h"
#include "wrappers/jni_motion_assistant_device_wrap.h"
#include "scanner_factory.h"
#include "wrappers/scan_state_callback.h"
#include "method_connection.h"

template<>
void deviceFoundCallback<JniMotionAssistantDevice>(void *subscriber, typename JniMotionAssistantDevice::object_ptr_t foundDevice) {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return;

    auto device = new JniMotionAssistantDevice(foundDevice);

    //creating java wrapper object for JniDevice
    jni::java_object<decltype(device)> deviceWrapObj(device);

    device->subscribeStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "deviceStateChanged"));
    device->subscribeBatteryLevelChanged(find_notifier<decltype(device)>(deviceWrapObj, "batteryLevelChanged"));
    device->subscribeMotionAssistantParamsChanged(find_notifier<decltype(device)>(deviceWrapObj, "motionAssistantParamsChanged"));
    device->subscribeMotionAssistantStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "motionAssistantStateChanged"));
    device->subscribeStimulationParamsChanged(find_notifier<decltype(device)>(deviceWrapObj, "stimulationParamsChanged"));
    device->subscribeStimulatorStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "stimulationStateChanged"));

    __android_log_print(ANDROID_LOG_VERBOSE, "DeviceConnectionJni",
                        "Device object created. Passing to java method");
    //pass wrapper object to callback function
    //subscriber must have onDeviceFound method with void(BfbDevice) signature
    auto subscriberClass = env->GetObjectClass((jobject) subscriber);
    auto callbackMethodSignature = std::string("(L") + jni::java_class_name<decltype(device)>() + ";)V";
    auto callbackMethod = env->GetMethodID(subscriberClass, "onDeviceFound", callbackMethodSignature.c_str());
    env->CallVoidMethod((jobject) subscriber, callbackMethod, static_cast<jobject>(deviceWrapObj));

    if (resCode == 1) jni::detach_thread();
}


extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDeviceConnector_createStimulationConnectionObj(
        JNIEnv *env, jobject instance, jobject context) {
    auto maConnectionObj = new MethodDeviceConnection<MotionAssistantDevice>(getNeuroConnectionScanner(context));
    auto globalSubscriberRef = env->NewGlobalRef(
            instance);
    maConnectionObj->subscribeScanStateChanged(
            [globalSubscriberRef](bool isScanning) {
                auto log = LoggerFactory::getCurrentPlatformLogger();
                log->debug("[%s] On scan state changed", __FUNCTION__);
                javaOnScanStateChanged(globalSubscriberRef, isScanning);
            });
    maConnectionObj->subscribeDeviceFound(
            [globalSubscriberRef](std::shared_ptr<MotionAssistantDevice> device) {
                auto log = LoggerFactory::getCurrentPlatformLogger();
                log->debug("[%s] On device found", __FUNCTION__);
                deviceFoundCallback<JniMotionAssistantDevice>(globalSubscriberRef, device);
            });
    return reinterpret_cast<jlong>(maConnectionObj);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDeviceConnector_deleteStimulationConnectionObj(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto stimulationConnection = (MethodDeviceConnection<MotionAssistantDevice> *) objPtr;
    delete stimulationConnection;
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDeviceConnector_getDeviceByAddress__JLjava_lang_String_2(
        JNIEnv *env, jobject instance, jlong objPtr, jstring address_) {

    const char *address = env->GetStringUTFChars(address_, 0);
    auto maConnection = reinterpret_cast<MethodDeviceConnection<MotionAssistantDevice> *>(objPtr);
    auto devicePtr = maConnection->getDeviceByAddress(address);
    env->ReleaseStringUTFChars(address_, address);

    if (!devicePtr) return NULL;
    auto device = new JniMotionAssistantDevice(devicePtr);

    jni::java_object<decltype(device)> deviceWrapObj(device);

    device->subscribeStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "deviceStateChanged"));
    device->subscribeBatteryLevelChanged(find_notifier<decltype(device)>(deviceWrapObj, "batteryLevelChanged"));
    device->subscribeMotionAssistantParamsChanged(find_notifier<decltype(device)>(deviceWrapObj, "motionAssistantParamsChanged"));
    device->subscribeMotionAssistantStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "motionAssistantStateChanged"));
    device->subscribeStimulationParamsChanged(find_notifier<decltype(device)>(deviceWrapObj, "stimulationParamsChanged"));
    device->subscribeStimulatorStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "stimulationStateChanged"));

    return  env->NewLocalRef(deviceWrapObj);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDeviceConnector_stopScan__J(JNIEnv *env,
                                                                                      jobject instance,
                                                                                      jlong objPtr) {

    auto stimulationConnection = (MethodDeviceConnection<MotionAssistantDevice> *) objPtr;
    stimulationConnection->stopScan();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDeviceConnector_startScan__JI(JNIEnv *env,
                                                                                        jobject instance,
                                                                                        jlong objPtr,
                                                                                        jint timeout) {

    auto stimulationConnection = (MethodDeviceConnection<MotionAssistantDevice> *) objPtr;
    stimulationConnection->startScan(timeout);

}


}