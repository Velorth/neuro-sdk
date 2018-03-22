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

#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/device/jni_device_param_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"

extern "C"
{

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_Device_init(JNIEnv *env, jobject instance) {

    auto deviceWrap = extract_pointer<JniDeviceWrap>(env, instance);
    deviceWrap->subscribeParameterChanged(
            find_notifier<decltype(deviceWrap)>(instance, "parameterChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_Device_deleteDevice(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniDeviceWrap>(env, instance);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_Device_connect(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    devicePtr->connect();
}


JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_Device_disconnect(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    devicePtr->disconnect();
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_Device_channels(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    auto deviceChannels = devicePtr->channels();
    return jni::to_obj_array(env, deviceChannels);
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_Device_commands(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    auto deviceCommands = devicePtr->commands();
    return jni::to_obj_array(env, deviceCommands);
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_Device_parameters(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    auto deviceParams = devicePtr->parameters();
    return jni::to_obj_array(env, deviceParams);
}

JNIEXPORT jboolean JNICALL
Java_ru_neurotech_neurosdk_Device_setParam(JNIEnv *env, jobject instance, jobject param,
                                           jobject value) {
    auto paramEnum = jni::enumFromJavaObj<Neuro::Parameter>(env, param);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_Device_readParam(JNIEnv *env, jobject instance, jobject param) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    auto paramEnum = jni::enumFromJavaObj<Neuro::Parameter>(env, param);
    return readDeviceParam(env, **devicePtr, paramEnum);
}

JNIEXPORT jboolean JNICALL
Java_ru_neurotech_neurosdk_Device_execute(JNIEnv *env, jobject instance, jobject cmd) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    try {
        auto result = devicePtr->execute(jni::enumFromJavaObj<Neuro::Command>(env, cmd));
        return static_cast<jboolean>(result);
    }
    catch (std::runtime_error &e){
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return JNI_FALSE;
    }
}

}

void JniDeviceWrap::subscribeParameterChanged(jobject stateChangedSubscriberRef) {
    parameterChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = parameterChangedGlobalSubscriberRef;
    this->object->setParamChangedCallback([weakReference](Neuro::Parameter parameter){
        sendNotification<Neuro::Parameter>(weakReference, parameter);
    });
}