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

#include "java_helper.h"
#include "cdevice.h"

extern "C"
{

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_Device_freeListenerHandle(JNIEnv *, jclass, jlong listenerHandle) {
    auto listenerHelperPtr = reinterpret_cast<ListenerHelper *>(listenerHandle);
    delete listenerHelperPtr;
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_Device_deviceConnect(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    throw_if_error(env, device_connect(device));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_Device_deviceDisconnect(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    throw_if_error(env, device_disconnect(device));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_Device_deviceDelete(JNIEnv *, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    device_delete(device);
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_Device_deviceAvailableChannels(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    ChannelInfoArray channelsArray;
    auto result = device_available_channels(device, &channelsArray);
    if (result != SDK_NO_ERROR){
        throw_if_error(env, result);
        return nullptr;
    }
    auto channelInfoClass = env->FindClass("Lcom/neuromd/neurosdk/channels/ChannelInfo;");
    if (channelInfoClass == nullptr){
        java_throw(env, "ChannelInfo class not found");
        return nullptr;
    }

    if (channelsArray.info_count > std::numeric_limits<jint>::max()){
        java_throw(env, "Too many elements in ChannelInfo array");
        return nullptr;
    }

    auto channelTypeClass = env->FindClass("Lcom/neuromd/neurosdk/channels/ChannelType;");
    if (channelTypeClass == nullptr){
        java_throw(env, "ChannelType enum not found");
        return nullptr;
    }

    auto channelInfoArray = to_obj_array(env,
                        channelInfoClass,
                        "(Lcom/neuromd/neurosdk/channels/ChannelInfo;Ljava/lang/String;J)V",
                        channelsArray.info_array,
                        static_cast<jint>(channelsArray.info_count),
                        [channelTypeClass](JNIEnv *env, jclass element_class, jmethodID constructor, ChannelInfo native_info){

        auto channelType = get_enum_field_ref(env, channelTypeClass, enum_name_map<ChannelType>::name(native_info.type).c_str());
        auto channelName = env->NewStringUTF(native_info.name);
        return env->NewObject(element_class, constructor, channelType, channelName, static_cast<jlong>(native_info.index));
    });

    free_ChannelInfoArray(channelsArray);
    return channelInfoArray;
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_Device_deviceAvailableCommands(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    CommandArray commandArray;
    auto result = device_available_commands(device, &commandArray);
    if (result != SDK_NO_ERROR){
        throw_if_error(env, result);
        return nullptr;
    }
    auto commandClass = env->FindClass("Lcom/neuromd/neurosdk/parameters/Command;");
    if (commandClass == nullptr){
        java_throw(env, "Command enum not found");
        return nullptr;
    }

    if (commandArray.cmd_array_size > std::numeric_limits<jint>::max()){
        java_throw(env, "Too many elements in Command array");
        return nullptr;
    }

    auto commandJavaArray = env->NewObjectArray(static_cast<jsize>(commandArray.cmd_array_size), commandClass, nullptr);

    for (auto i = 0 ; i < commandArray.cmd_array_size; ++i) {
        env->PushLocalFrame(1);
        auto commandJava = get_enum_field_ref(env, commandClass, enum_name_map<Command>::name(commandArray.cmd_array[i]).c_str());
        env->SetObjectArrayElement(commandJavaArray, i, commandJava);
        env->PopLocalFrame(nullptr);
    }

    free_CommandArray(commandArray);
    return commandJavaArray;
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_Device_deviceAvailableParameters(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    ParamInfoArray paramInfoArray;
    auto result = device_available_parameters(device, &paramInfoArray);
    if (result != SDK_NO_ERROR){
        throw_if_error(env, result);
        return nullptr;
    }
    auto parameterClass = env->FindClass("Lcom/neuromd/neurosdk/parameters/Parameter;");
    if (parameterClass == nullptr){
        java_throw(env, "Parameter class not found");
        return nullptr;
    }

    if (paramInfoArray.info_count > std::numeric_limits<jint>::max()){
        java_throw(env, "Too many elements in ParamInfo array");
        return nullptr;
    }

    auto paramNameClass = env->FindClass("Lcom/neuromd/neurosdk/parameters/ParameterName;");
    if (paramNameClass == nullptr){
        java_throw(env, "ParamName enum not found");
        return nullptr;
    }

    auto paramAccessClass = env->FindClass("Lcom/neuromd/neurosdk/parameters/ParameterAccess;");
    if (paramAccessClass == nullptr){
        java_throw(env, "ParamAccess enum not found");
        return nullptr;
    }

    auto paramTypeClass = env->FindClass("Lcom/neuromd/neurosdk/parameters/ParameterType;");
    if (paramTypeClass == nullptr){
        java_throw(env, "ParamType enum not found");
        return nullptr;
    }

    auto paramJavaArray = to_obj_array(env,
                                       parameterClass,
                                       "(Lcom/neuromd/neurosdk/parameters/ParameterName;Lcom/neuromd/neurosdk/parameters/ParameterAccess;Lcom/neuromd/neurosdk/parameters/ParameterType;)V",
                                       paramInfoArray.info_array,
                                       static_cast<jint>(paramInfoArray.info_count),
                                       [paramNameClass, paramAccessClass, paramTypeClass](JNIEnv *env, jclass element_class, jmethodID constructor, ParameterInfo native_info){

        auto paramName = get_enum_field_ref(env, paramNameClass, enum_name_map<Parameter>::name(native_info.parameter).c_str());

        return env->NewObject(element_class, constructor, paramName);
    });

    free_ParamInfoArray(paramInfoArray);
    return paramJavaArray;
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_Device_deviceExecute(JNIEnv *env, jclass type, jlong devicePtr, jobject command) {

    // TODO

}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_Device_deviceSubscribeParamChanged(JNIEnv *env, jclass type, jlong devicePtr) {

    // TODO

}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_Device_deviceSubscribeDoubleChannelDataReceived(JNIEnv *env, jclass type,
                                                                          jlong devicePtr,
                                                                          jobject info) {

    // TODO

}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_Device_deviceSubscribeIntChannelDataReceived(JNIEnv *env, jclass type,
                                                                       jlong devicePtr,
                                                                       jobject info) {

    // TODO

}

}