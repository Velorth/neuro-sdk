#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/channels/jni_battery_channel_wrap.h"
#include "channels/battery_channel.h"


extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_create(JNIEnv *env, jclass type,
                                                          jobject device) {
    return createChannelFromDevice<JniBatteryChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_info(JNIEnv *env, jobject instance) {

    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    auto channelInfo = &batteryChannelWrap->info();
    jni::java_object<decltype(channelInfo)> nativeChannelInfo(channelInfo);
    return env->NewLocalRef(nativeChannelInfo);
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_init(JNIEnv *env, jobject instance) {

    auto batteryChannelWrap = extract_pointer<JniBatteryChannelWrap>(env, instance);
    batteryChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(batteryChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniBatteryChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);

}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_setSamplingFrequency(JNIEnv *env,
                                                                        jobject instance,
                                                                        jfloat frequency) {
    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    try {
        batteryChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_samplingFrequency(JNIEnv *env,
                                                                     jobject instance) {
    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    return batteryChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    return saturation_cast<jlong>(batteryChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    return saturation_cast<jlong>(batteryChannelWrap->totalLength());
}

JNIEXPORT jintArray JNICALL
Java_ru_neurotech_neurosdk_channels_BatteryChannel_readData(JNIEnv *env, jobject instance,
                                                            jlong offset, jlong length) {
    auto &batteryChannelWrap = *extract_pointer<JniBatteryChannelWrap>(env, instance);
    auto data = batteryChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                             saturation_cast<Neuro::data_length_t>(length));
    if (data.size() > std::numeric_limits<jsize>::max()) {
        jni::java_throw(env,
                        "java/lang/ArrayIndexOutOfBoundsException",
                        std::runtime_error("Requested data array is too big"));
        return nullptr;
    }

    jintArray dataArray = env->NewIntArray(static_cast<jsize>(data.size()));
    if (dataArray == nullptr)
        return nullptr;

    env->SetIntArrayRegion(dataArray, 0, static_cast<jsize>(data.size()), data.data());
    return dataArray;
}

}

void JniBatteryChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}

