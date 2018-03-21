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
    deviceWrap->subscribeStateChanged(
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
    if (deviceChannels.size() > std::numeric_limits<jsize>::max()) {
        jni::java_throw(env,
                        "java/lang/ArrayIndexOutOfBoundsException",
                        std::runtime_error("Channels array is too big"));
        return nullptr;
    }
    auto commandClass = jni::java_object<Neuro::ChannelInfo>::java_class();
    auto commandsArray = env->NewObjectArray(static_cast<jsize>(deviceChannels.size()),
                                             commandClass,
                                             NULL);

    for (auto it = deviceChannels.begin(); it != deviceChannels.end(); ++it) {
        env->SetObjectArrayElement(commandsArray,
                                   static_cast<jsize>(it - deviceChannels.begin()),
                                   env->NewLocalRef(jni::java_object<Neuro::ChannelInfo>(*it)));
    }

    return commandsArray;
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_Device_commands(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    auto deviceCommands = devicePtr->commands();
    if (deviceCommands.size() > std::numeric_limits<jsize>::max()) {
        jni::java_throw(env,
                        "java/lang/ArrayIndexOutOfBoundsException",
                        std::runtime_error("Commands array is too big"));
        return nullptr;
    }
    auto commandClass = jni::java_object<Neuro::Command>::java_class();
    auto commandsArray = env->NewObjectArray(static_cast<jsize>(deviceCommands.size()),
                                             commandClass,
                                             NULL);

    for (auto it = deviceCommands.begin(); it != deviceCommands.end(); ++it) {
       env->SetObjectArrayElement(commandsArray,
                                  static_cast<jsize>(it - deviceCommands.begin()),
                                  env->NewLocalRef(jni::java_object<Neuro::Command>(*it)));
    }

    return commandsArray;
}

JNIEXPORT jboolean JNICALL
Java_ru_neurotech_neurosdk_Device_setParam(JNIEnv *env, jobject instance, jobject param,
                                           jobject value) {

    // TODO

}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_Device_readParam(JNIEnv *env, jobject instance, jobject param) {

    // TODO

}

JNIEXPORT jboolean JNICALL
Java_ru_neurotech_neurosdk_Device_execute(JNIEnv *env, jobject instance, jobject cmd) {

    // TODO

}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_Device_parameters(JNIEnv *env, jobject instance) {
    auto& devicePtr = *extract_pointer<JniDeviceWrap>(env, instance);
    auto deviceParams = devicePtr->parameters();
    if (deviceParams.size() > std::numeric_limits<jsize>::max()) {
        jni::java_throw(env,
                        "java/lang/ArrayIndexOutOfBoundsException",
                        std::runtime_error("Commands array is too big"));
        return nullptr;
    }
    using param_pair = typename decltype(deviceParams)::value_type;
    auto paramsClass = jni::java_object<param_pair>::java_class();
    auto paramsArray = env->NewObjectArray(static_cast<jsize>(deviceParams.size()),
                                             paramsClass,
                                             NULL);

    for (auto it = deviceParams.begin(); it != deviceParams.end(); ++it) {
        env->SetObjectArrayElement(paramsArray,
                                   static_cast<jsize>(it - deviceParams.begin()),
                                   env->NewLocalRef(jni::java_object<param_pair>(*it)));
    }

    return paramsArray;
}

/*JNIEXPORT jobjectArray JNICALL
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
}*/

}

void JniDeviceWrap::subscribeStateChanged(jobject stateChangedSubscriberRef) {
    deviceStateChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = deviceStateChangedGlobalSubscriberRef;
    this->object->setParamChangedCallback([weakReference](Neuro::Parameter parameter){
        sendNotification<Neuro::Parameter>(weakReference, parameter);
    });
}