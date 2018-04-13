#include "wrappers/channels/jni_mems_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_create(JNIEnv *env, jclass type,
                                                         jobject device) {
    return createChannelFromDevice<JniMEMSChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_info(JNIEnv *env, jobject instance) {

    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    auto channelInfo = &memsChannelWrap->info();
    jni::java_object<decltype(channelInfo)> nativeChannelInfo(channelInfo);
    return env->NewLocalRef(nativeChannelInfo);
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_init(JNIEnv *env, jobject instance) {

    auto memsChannelWrap = extract_pointer<JniMEMSChannelWrap>(env, instance);
    memsChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(memsChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniMEMSChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    auto devicePtr = memsChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    jni::java_object<decltype(deviceWrap)> deviceWrapObj(deviceWrap);
    return env->NewLocalRef(deviceWrapObj);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_setSamplingFrequency(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloat frequency) {
    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    try {
        memsChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    return memsChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    return saturation_cast<jlong>(memsChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    return saturation_cast<jlong>(memsChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_channels_MEMSChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    auto &memsChannelWrap = *extract_pointer<JniMEMSChannelWrap>(env, instance);
    auto data = memsChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                            saturation_cast<Neuro::data_length_t>(length));
    return jni::to_obj_array(env, data);
}

}

void JniMEMSChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}

template<>
template<>
jni::java_object<Neuro::MEMS>::java_object(const Neuro::MEMS &mems):
        nativeObj(mems){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::MEMS>());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class,
                               objectClassConstructor,
                               mems.accelerometer.X,
                               mems.accelerometer.Y,
                               mems.accelerometer.Z,
                               mems.gyroscope.X,
                               mems.gyroscope.Y,
                               mems.gyroscope.Z));
    });
}