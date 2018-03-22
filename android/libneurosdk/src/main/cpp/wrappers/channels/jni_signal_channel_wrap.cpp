#include "wrappers/channels/jni_signal_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_create(JNIEnv *env, jclass type,
                                                         jobject device) {
    return createChannelFromDevice<JniSignalChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_info(JNIEnv *env, jobject instance) {

    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    auto channelInfo = &signalChannelWrap->info();
    jni::java_object<decltype(channelInfo)> nativeChannelInfo(channelInfo);
    return env->NewLocalRef(nativeChannelInfo);
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_init(JNIEnv *env, jobject instance) {

    auto signalChannelWrap = extract_pointer<JniSignalChannelWrap>(env, instance);
    signalChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(signalChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniSignalChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    auto devicePtr = signalChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    jni::java_object<decltype(deviceWrap)> deviceWrapObj(deviceWrap);
    return env->NewLocalRef(deviceWrapObj);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_setSamplingFrequency(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloat frequency) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    try {
        signalChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    return signalChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    return saturation_cast<jlong>(signalChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    return saturation_cast<jlong>(signalChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    auto data = signalChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                            saturation_cast<Neuro::data_length_t>(length));
    return jni::to_obj_array(env, data);
}

}

void JniSignalChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}
