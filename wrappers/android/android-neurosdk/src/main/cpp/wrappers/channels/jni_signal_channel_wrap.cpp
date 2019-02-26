#include "java_helper.h"
#include "csignal-channel.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createSignalDoubleChannel(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    auto signalChannel = create_SignalDoubleChannel(device);
    if (signalChannel == nullptr){
        char errorMsg[256];
        sdk_last_error_msg(errorMsg, 256);
        java_throw(env, errorMsg);
        return 0;
    }
    return reinterpret_cast<jlong>(signalChannel);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createSignalDoubleChannelInfo(JNIEnv *env, jclass, jlong devicePtr, jobject info) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try{
        auto channelInfo = channel_info_from_jobject(env, info);
        auto signalChannel = create_SignalDoubleChannel_info(device, channelInfo);
        if (signalChannel == nullptr){
            char errorMsg[256];
            sdk_last_error_msg(errorMsg, 256);
            java_throw(env, errorMsg);
            return 0;
        }
        return reinterpret_cast<jlong>(signalChannel);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return 0;
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createSignalDoubleChannelInfoFilters(JNIEnv *env, jclass, jlong devicePtr, jobject info, jobjectArray filtersArray) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try{
        auto channelInfo = channel_info_from_jobject(env, info);
        auto filters = filters_from_java_array(env, filtersArray);
        auto signalChannel = create_SignalDoubleChannel_info_filters(device, channelInfo, filters.data(), filters.size());
        if (signalChannel == nullptr){
            char errorMsg[256];
            sdk_last_error_msg(errorMsg, 256);
            java_throw(env, errorMsg);
            return 0;
        }
        return reinterpret_cast<jlong>(signalChannel);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return 0;
    }
}


extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createSignalDoubleChannelFilters(JNIEnv *env, jclass, jlong devicePtr, jobjectArray filtersArray) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try{
        auto filters = filters_from_java_array(env, filtersArray);
        auto signalChannel = create_SignalDoubleChannel_filters(device, filters.data(), filters.size());
        if (signalChannel == nullptr){
            char errorMsg[256];
            sdk_last_error_msg(errorMsg, 256);
            java_throw(env, errorMsg);
            return 0;
        }
        return reinterpret_cast<jlong>(signalChannel);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return 0;
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_SignalDoubleChannelGetBufferSize(JNIEnv *env, jclass, jlong signalChannelPtr) {
    auto signalChannel = reinterpret_cast<SignalDoubleChannel *>(signalChannelPtr);
    size_t bufferSize;
    throw_if_error(env, SignalDoubleChannel_get_buffer_size(signalChannel, &bufferSize));
    return bufferSize;
}
