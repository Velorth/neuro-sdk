#include "wrappers/channels/jni_signal_channel_wrap.h"
#include "wrappers/channels/jni_spectrum_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_createFromChannel(JNIEnv *env, jclass type,
                                                         jobject src_channel) {
    auto& srcChannelWrapPtr = *extract_pointer<JniSignalChannelWrap>(env, src_channel);
    try {
        auto channel = std::make_shared<typename JniSpectrumChannelWrap::obj_t>(*srcChannelWrapPtr);
        auto channelWrap = new JniSpectrumChannelWrap(channel);
        return reinterpret_cast<jlong>(channelWrap);
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_ERROR, "CreateChannelFromChannel",
                            "Error creating channel: %s", e.what());
        jni::java_throw(env, "java/lang/IllegalArgumentException", e);
        return 0;
    }
}

JNIEXPORT jobject
JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_info(JNIEnv *env, jobject instance) {

    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    auto channelInfo = &spectrumChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);
}

JNIEXPORT void
JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_init(JNIEnv *env, jobject instance) {

    auto spectrumChannelWrap = extract_pointer<JniSpectrumChannelWrap>(env, instance);
    spectrumChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(spectrumChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniSpectrumChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    auto devicePtr = spectrumChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_setSamplingFrequency(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloat frequency) {
    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    try {
        spectrumChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    return spectrumChannelWrap->samplingFrequency();
}

JNIEXPORT jdouble JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_hzPerSpectrumSample(JNIEnv *env,
                                                                     jobject instance) {
    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    return spectrumChannelWrap->hzPerSpectrumSample();
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    return saturation_cast<jlong>(spectrumChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
    return saturation_cast<jlong>(spectrumChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    try {
        auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
        auto data = spectrumChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

JNIEXPORT jdoubleArray JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_readFast(JNIEnv *env, jobject instance,
                                                          jlong offset, jlong length) {
    try {
        auto &spectrumChannelWrap = *extract_pointer<JniSpectrumChannelWrap>(env, instance);
        auto data = spectrumChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                saturation_cast<Neuro::data_length_t>(length));

        auto dataSize = saturation_cast<jsize>(data.size());
        auto doubleArray = env->NewDoubleArray(dataSize);

        env->SetDoubleArrayRegion(doubleArray, 0, dataSize, data.data());
        return doubleArray;
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

}

void JniSpectrumChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    mListener = this->object->subscribeLengthChanged([weakReference](auto length){
        JNIEnv *env;
        jni::get_env(&env);
        env->PushLocalFrame(2);
        sendNotification<long>(env, weakReference, length);
        env->PopLocalFrame(nullptr);
    });
}
