#include "wrappers/channels/jni_resistance_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_createWithInfo(JNIEnv *env, jclass type,
                                                                jobject device, jobject info) {
    return createChannelFromDevice<JniResistanceChannelWrap>(env, device, info);
}

JNIEXPORT jobject
JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_info(JNIEnv *env, jobject instance) {

    auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
    auto channelInfo = &resistanceChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);
}

JNIEXPORT void
JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_init(JNIEnv *env, jobject instance) {

    auto resistanceChannelWrap = extract_pointer<JniResistanceChannelWrap>(env, instance);
    resistanceChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(resistanceChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniResistanceChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
    auto devicePtr = resistanceChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_setSamplingFrequency(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloat frequency) {
    auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
    try {
        resistanceChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
    return resistanceChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
    return saturation_cast<jlong>(resistanceChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
    return saturation_cast<jlong>(resistanceChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    try {
        auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
        auto data = resistanceChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

JNIEXPORT jdoubleArray JNICALL
Java_com_neuromd_neurosdk_channels_ResistanceChannel_readFast(JNIEnv *env, jobject instance,
                                                          jlong offset, jlong length) {
    try {
        auto &resistanceChannelWrap = *extract_pointer<JniResistanceChannelWrap>(env, instance);
        auto data = resistanceChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
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

void JniResistanceChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
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
