#include "java_helper.h"


void onTotalLengthChanged(AnyChannel *channel, size_t length, void *listener_helper){
    auto listenerHelper = reinterpret_cast<ListenerHelper *>(listener_helper);
    listenerHelper->notify(reinterpret_cast<jlong>(channel), static_cast<jlong>(length));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_freeListenerHandle(JNIEnv *, jclass, jlong listenerHandle) {
    auto listenerHelperPtr = reinterpret_cast<ListenerHelper *>(listenerHandle);
    delete listenerHelperPtr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelDelete(JNIEnv *, jclass, jlong anyChannelPtr) {
    auto anyChannel = reinterpret_cast<AnyChannel *>(anyChannelPtr);
    AnyChannel_delete(anyChannel);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelGetInfo(JNIEnv *env, jclass, jlong anyChannelPtr) {
    auto anyChannel = reinterpret_cast<AnyChannel *>(anyChannelPtr);
    ChannelInfo channelInfo;
    auto result = AnyChannel_get_info(anyChannel, &channelInfo);
    if (result != SDK_NO_ERROR){
        throw_if_error(env, result);
        return nullptr;
    }

    try {
        return java_channel_info_from_native(env, channelInfo);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelGetSamplingFrequency(JNIEnv *env, jclass, jlong anyChannelPtr) {
    auto anyChannel = reinterpret_cast<AnyChannel *>(anyChannelPtr);
    float samplingFrequency;
    throw_if_error(env, AnyChannel_get_sampling_frequency(anyChannel, &samplingFrequency));
    return samplingFrequency;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelSubscribeLengthChanged(JNIEnv *env, jobject instance, jlong anyChannelPtr) {
    auto anyChannel = reinterpret_cast<AnyChannel *>(anyChannelPtr);
    try {
        auto helperPtr = make_listener_helper(env, instance, "onTotalLengthChanged", "(JJ)V");
        throw_if_error(env, AnyChannel_add_length_callback(anyChannel, &onTotalLengthChanged, helperPtr->listnerHandlePointer(), helperPtr));
        return reinterpret_cast<jlong>(helperPtr);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return 0;
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelGetTotalLength(JNIEnv *env, jclass, jlong anyChannelPtr) {
    auto anyChannel = reinterpret_cast<AnyChannel *>(anyChannelPtr);
    size_t totalLength;
    throw_if_error(env, AnyChannel_get_total_length(anyChannel, &totalLength));
    return totalLength;
}