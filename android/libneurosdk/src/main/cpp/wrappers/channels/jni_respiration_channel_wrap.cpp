#include "wrappers/channels/jni_respiration_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_create(JNIEnv *env, jclass type,
                                                         jobject device) {
    return createChannelFromDevice<JniRespirationChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_info(JNIEnv *env, jobject instance) {

    auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
    auto channelInfo = &respChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_init(JNIEnv *env, jobject instance) {

    auto respChannelWrap = extract_pointer<JniRespirationChannelWrap>(env, instance);
    respChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(respChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniRespirationChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
    auto devicePtr = respChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);;
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_setSamplingFrequency(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloat frequency) {
    auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
    try {
        respChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
    return respChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
    return saturation_cast<jlong>(respChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
    return saturation_cast<jlong>(respChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_channels_RespirationChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    try {
        auto &respChannelWrap = *extract_pointer<JniRespirationChannelWrap>(env, instance);
        auto data = respChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                              saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

}

void JniRespirationChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}
