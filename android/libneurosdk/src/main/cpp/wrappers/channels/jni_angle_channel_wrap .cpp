#include "wrappers/channels/jni_angle_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_create(JNIEnv *env, jclass type,
                                                         jobject device) {
    return createChannelFromDevice<JniAngleChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_info(JNIEnv *env, jobject instance) {

    auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
    auto channelInfo = &angleChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);;
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_init(JNIEnv *env, jobject instance) {

    auto angleChannelWrap = extract_pointer<JniAngleChannelWrap>(env, instance);
    angleChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(angleChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniAngleChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
    auto devicePtr = angleChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_setSamplingFrequency(JNIEnv *env,
                                                                       jobject instance,
                                                                       jfloat frequency) {
    auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
    try {
        angleChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
    return angleChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
    return saturation_cast<jlong>(angleChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
    return saturation_cast<jlong>(angleChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_channels_AngleChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    try {
        auto &angleChannelWrap = *extract_pointer<JniAngleChannelWrap>(env, instance);
        auto data = angleChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

}

void JniAngleChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}

template<>
template<>
jni::java_object<Neuro::Quaternion>::java_object(const Neuro::Quaternion &quaternion):
        nativeObj(quaternion){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::Quaternion>());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class,
                               objectClassConstructor,
                               quaternion.W,
                               quaternion.X,
                               quaternion.Y,
                               quaternion.Z));
    });
}