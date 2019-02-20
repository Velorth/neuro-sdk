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

void onParamChanged(Device *device, Parameter parameter, void *listener_helper){
    auto listenerHelper = reinterpret_cast<ListenerHelper *>(listener_helper);
    listenerHelper->execInJavaThread([parameter](JNIEnv *env){
        const auto& paramEnum = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/ParameterName;");
        auto parameterName = get_enum_field_ref(env, paramEnum, get_enum_name(parameter).c_str());
        listenerHelper->notify(reinterpret_cast<jlong>(device), parameterName);
    });
}

void onDoubleDataReceived(Device *device, ChannelInfo info, DoubleDataArray data, void *listener_helper){
    auto listenerHelper = reinterpret_cast<ListenerHelper *>(listener_helper);
    listenerHelper->execInJavaThread([info, data](JNIEnv *env){

        listenerHelper->notify(reinterpret_cast<jlong>(device), ...);
    });
}

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

    if (channelsArray.info_count > std::numeric_limits<jint>::max()){
        free_ChannelInfoArray(channelsArray);
        java_throw(env, "Too many elements in ChannelInfo array");
        return nullptr;
    }

    try {
        const auto& channelInfoClass = global_class_refs().fromClassName("com/neuromd/neurosdk/channels/ChannelInfo");
        const auto& channelTypeClass = global_class_refs().fromClassName("com/neuromd/neurosdk/channels/ChannelType");

        auto channelInfoArray = to_obj_array(env,
                                             channelInfoClass,
                                             "(Lcom/neuromd/neurosdk/channels/ChannelInfo;Ljava/lang/String;J)V",
                                             channelsArray.info_array,
                                             static_cast<jint>(channelsArray.info_count),
                                             [&channelTypeClass](JNIEnv *env, jclass element_class,
                                                                jmethodID constructor,
                                                                ChannelInfo native_info) {
            auto channelType = get_enum_field_ref(env, channelTypeClass, get_enum_name(native_info.type).c_str());
            auto channelName = env->NewStringUTF(native_info.name);
            return env->NewObject(element_class, constructor, channelType, channelName, static_cast<jlong>(native_info.index));
        });

        free_ChannelInfoArray(channelsArray);
        return channelInfoArray;
    }
    catch (std::exception &e){
        free_ChannelInfoArray(channelsArray);
        java_throw(env, e.what());
        return nullptr;
    }
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

    if (commandArray.cmd_array_size > std::numeric_limits<jint>::max()) {
        free_CommandArray(commandArray);
        java_throw(env, "Too many elements in Command array");
        return nullptr;
    }

    try {
        const auto &commandClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/Command");
        auto commandJavaArray = env->NewObjectArray(static_cast<jsize>(commandArray.cmd_array_size), commandClass, nullptr);

        for (auto i = 0; i < commandArray.cmd_array_size; ++i) {
            env->PushLocalFrame(1);
            auto commandJava = get_enum_field_ref(env, commandClass, get_enum_name(commandArray.cmd_array[i]).c_str());
            env->SetObjectArrayElement(commandJavaArray, i, commandJava);
            env->PopLocalFrame(nullptr);
        }

        free_CommandArray(commandArray);
        return commandJavaArray;
    }
    catch (std::exception &e){
        free_CommandArray(commandArray);
        java_throw(env, e.what());
        return nullptr;
    }
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

    if (paramInfoArray.info_count > std::numeric_limits<jint>::max()){
        free_ParamInfoArray(paramInfoArray);
        java_throw(env, "Too many elements in ParamInfo array");
        return nullptr;
    }

    try {
        const auto &parameterClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/Parameter;");
        const auto &paramNameClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/ParameterName;");
        const auto &paramAccessClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/ParameterAccess;");
        const auto &paramTypeClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/ParameterType;");

        auto paramJavaArray = to_obj_array(env,
                                           parameterClass,
                                           "(Lcom/neuromd/neurosdk/parameters/ParameterName;Lcom/neuromd/neurosdk/parameters/ParameterAccess;Lcom/neuromd/neurosdk/parameters/ParameterType;)V",
                                           paramInfoArray.info_array,
                                           static_cast<jint>(paramInfoArray.info_count),
                                           [&paramNameClass, &paramAccessClass, &paramTypeClass](
                                                   JNIEnv *env, jclass element_class,
                                                   jmethodID constructor,
                                                   ParameterInfo native_info) {
            auto paramName = get_enum_field_ref(env, paramNameClass, get_enum_name(native_info.parameter).c_str());
            return env->NewObject(element_class, constructor, paramName, paramAccessClass, paramTypeClass);
        });

        free_ParamInfoArray(paramInfoArray);
        return paramJavaArray;
    }
    catch (std::exception &e){
        free_ParamInfoArray(paramInfoArray);
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_Device_deviceExecute(JNIEnv *env, jclass, jlong devicePtr, jobject java_command) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    auto command = enum_from_java_obj<Command>(env, java_command);
    throw_if_error(env, device_execute(device, command));
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_Device_deviceSubscribeParamChanged(JNIEnv *env, jobject instance, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    auto helperPtr = make_listener_helper(env, instance, "onParameterChanged", "(JLcom/neuromd/neurosdk/parameters/Parameter;)V");
    throw_if_error(env, device_subscribe_param_changed(device, &onParamChanged, helperPtr->listnerHandlePointer(), helperPtr));
    return reinterpret_cast<jlong>(helperPtr);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_Device_deviceSubscribeDoubleChannelDataReceived(JNIEnv *env, jobject instance, jlong devicePtr, jobject javaChannelInfo) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try {
        auto helperPtr = make_listener_helper(env, instance, "onDoubleDataReceived", "(JLcom/neuromd/neurosdk/channels/ChannelInfo;[D)V");
        auto channelInfo = channel_info_from_jobject(env, javaChannelInfo);
        throw_if_error(env, device_subscribe_double_channel_data_received(device, channelInfo, &onDoubleDataReceived, helperPtr->listnerHandlePointer(), helperPtr));
        return reinterpret_cast<jlong>(helperPtr);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return 0;
    }
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_Device_deviceSubscribeIntChannelDataReceived(JNIEnv *env, jobject instance, jlong devicePtr, jobject info) {

    // TODO

}

}