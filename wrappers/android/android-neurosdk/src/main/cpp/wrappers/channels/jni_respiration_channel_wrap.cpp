#include "java_helper.h"
#include "crespiration-channel.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_RespirationChannel_createRespirationDoubleChannel(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try{
        auto channelInfo = channel_info_from_jobject(env, info);
        auto resistChannel = create_RespirationDoubleChannel(device);
        if (resistChannel == nullptr){
            char errorMsg[256];
            sdk_last_error_msg(errorMsg, 256);
            java_throw(env, errorMsg);
            return 0;
        }
        return reinterpret_cast<jlong>(resistChannel);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return 0;
    }
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_RespirationChannel_RespirationDoubleChannelGetBufferSize(JNIEnv *env, jclass, jlong resistChannelPtr) {
    auto resistChannel = reinterpret_cast<RespirationDoubleChannel *>(resistChannelPtr);
    size_t bufferSize;
    throw_if_error(env, RespirationDoubleChannel_get_buffer_size(resistChannel, &bufferSize));
    return bufferSize;
}
