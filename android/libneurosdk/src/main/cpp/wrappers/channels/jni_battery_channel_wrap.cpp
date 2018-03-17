#include <saturation_cast.h>
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

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniBatteryChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);

}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_setSamplingFrequency(JNIEnv *env,
                                                                        jobject instance,
                                                                        jfloat frequency) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    try{
        batteryChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e){
        auto exceptionClass = env->FindClass("java/lang/UnsupportedOperationException");
        if (exceptionClass == nullptr) {
            return;
        }
        env->ThrowNew(exceptionClass, e.what());
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_samplingFrequency(JNIEnv *env,
                                                                     jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    return batteryChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_availableLength(JNIEnv *env, jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    return saturation_cast<jlong>(batteryChannelWrap->availableLength());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_totalLength(JNIEnv *env, jobject instance) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    return saturation_cast<jlong>(batteryChannelWrap->totalLength());
}

JNIEXPORT jintArray JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_readData(JNIEnv *env, jobject instance,
                                                            jlong offset, jlong length) {
    auto batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    auto data = batteryChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                             saturation_cast<Neuro::data_length_t>(length));
    if (data.size() > std::numeric_limits<jsize>::max()){
        auto exceptionClass = env->FindClass("java/lang/ArrayIndexOutOfBoundsException");
        if (exceptionClass == nullptr) {
            return nullptr;
        }
        env->ThrowNew(exceptionClass, "Requested data array is too big");
        return nullptr;
    }

    jintArray dataArray = env->NewIntArray(static_cast<jsize>(data.size()));
    if (dataArray == nullptr)
        return nullptr;

    env->SetIntArrayRegion(dataArray, 0, static_cast<jsize>(data.size()), data.data());
}

}