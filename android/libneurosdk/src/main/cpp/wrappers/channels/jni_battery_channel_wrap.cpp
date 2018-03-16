#include "wrappers/jni_device_wrap.h"
#include "wrappers/channels/jni_battery_channel_wrap.h"
#include "channels/battery_channel.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_create(JNIEnv *env, jclass type,
                                                          jobject device) {

    return createChannelFromDevice<JniBatteryChannelWrap>(env, device);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_setSamplingFrequency(JNIEnv *env,
                                                                        jobject instance,
                                                                        jfloat frequency) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_samplingFrequency(JNIEnv *env,
                                                                     jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_availableLength(JNIEnv *env, jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_totalLength(JNIEnv *env, jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_readData(JNIEnv *env, jobject instance,
                                                            jlong offset, jlong length) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
}

}