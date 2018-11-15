#include "wrappers/channels/jni_signal_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_create(JNIEnv *env, jclass type,
                                                         jobject device) {
    return createChannelFromDevice<JniSignalChannelWrap>(env, device);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createWithInfo(JNIEnv *env, jclass type,
                                                                jobject device, jobject info) {
    return createChannelFromDevice<JniSignalChannelWrap>(env, device, info);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createWithFiltersAndInfo(JNIEnv *env, jclass type,
                                                                jobject device, jobjectArray filters, jobject info) {
    return createChannelFromDevice<JniSignalChannelWrap>(env, device, info, filters);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_createWithFilters(JNIEnv *env, jclass type,
                                                                          jobject device, jobjectArray filters) {
    return createChannelFromDevice<JniSignalChannelWrap>(env, device, filters);
}

JNIEXPORT jobject
JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_info(JNIEnv *env, jobject instance) {

    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    auto channelInfo = &signalChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);
}

JNIEXPORT void
JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_init(JNIEnv *env, jobject instance) {

    auto signalChannelWrap = extract_pointer<JniSignalChannelWrap>(env, instance);
    signalChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(signalChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniSignalChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    auto devicePtr = signalChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);
}

JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    return signalChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    return saturation_cast<jlong>(signalChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
    return saturation_cast<jlong>(signalChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    try {
        auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
        auto data = signalChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

JNIEXPORT jdoubleArray JNICALL
Java_com_neuromd_neurosdk_channels_SignalChannel_readFast(JNIEnv *env, jobject instance,
                                                          jlong offset, jlong length) {
    try {
        auto &signalChannelWrap = *extract_pointer<JniSignalChannelWrap>(env, instance);
        auto data = signalChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
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

void JniSignalChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    mListener = this->object->subscribeLengthChanged([weakReference](auto length) {
        JNIEnv *env;
        jni::get_env(&env);
        env->PushLocalFrame(1);
        sendNotification<long>(env, weakReference, length);
        env->PopLocalFrame(nullptr);
    });
}
