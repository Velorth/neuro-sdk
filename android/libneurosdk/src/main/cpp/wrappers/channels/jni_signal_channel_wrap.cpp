#include "wrappers/channels/jni_signal_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/jni_device_wrap.h"

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
        auto exceptionClass = env->FindClass("java/lang/UnsupportedOperationException");
        if (exceptionClass == nullptr) {
            return;
        }
        env->ThrowNew(exceptionClass, e.what());
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

JNIEXPORT jdoubleArray JNICALL
Java_ru_neurotech_neurosdk_channels_SignalChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    auto data = signalChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                            saturation_cast<Neuro::data_length_t>(length));
    if (data.size() > std::numeric_limits<jsize>::max()) {
        auto exceptionClass = env->FindClass("java/lang/ArrayIndexOutOfBoundsException");
        if (exceptionClass == nullptr) {
            return nullptr;
        }
        env->ThrowNew(exceptionClass, "Requested data array is too big");
        return nullptr;
    }

    auto dataArray = env->NewDoubleArray(static_cast<jsize>(data.size()));
    if (dataArray == nullptr)
        return nullptr;

    env->SetDoubleArrayRegion(dataArray, 0, static_cast<jsize>(data.size()), data.data());
    return dataArray;
}

}

void JniSignalChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}
