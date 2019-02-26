#include "java_helper.h"
#include "cbattery-channel.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_BatteryChannel_createBatteryIntChannel(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    auto batteryChannel = create_BatteryIntChannel(device);
    if (batteryChannel == nullptr){
        char errorMsg[256];
        sdk_last_error_msg(errorMsg, 256);
        java_throw(env, errorMsg);
        return 0;
    }
    return reinterpret_cast<jlong>(batteryChannel);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_BatteryChannel_BatteryIntChannelGetBufferSize(JNIEnv *env, jclass, jlong batteryChannelPtr) {
    auto batteryChannel = reinterpret_cast<BatteryIntChannel *>(batteryChannelPtr);
    size_t bufferSize;
    throw_if_error(env, BatteryIntChannel_get_buffer_size(batteryChannel, &bufferSize));
    return bufferSize;
}