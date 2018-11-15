#include "wrappers/channels/jni_angle_channel_wrap.h"
#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_create(JNIEnv *env, jclass type,
                                                         jobject device) {
    return createChannelFromDevice<JniOrientationChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_info(JNIEnv *env, jobject instance) {

    auto &angleChannelWrap = *extract_pointer<JniOrientationChannelWrap>(env, instance);
    auto channelInfo = &angleChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);;
}

JNIEXPORT void
JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_init(JNIEnv *env, jobject instance) {

    auto angleChannelWrap = extract_pointer<JniOrientationChannelWrap>(env, instance);
    angleChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(angleChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniOrientationChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniOrientationChannelWrap>(env, instance);
    auto devicePtr = angleChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);
}

JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_samplingFrequency(JNIEnv *env,
                                                                    jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniOrientationChannelWrap>(env, instance);
    return angleChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniOrientationChannelWrap>(env, instance);
    return saturation_cast<jlong>(angleChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &angleChannelWrap = *extract_pointer<JniOrientationChannelWrap>(env, instance);
    return saturation_cast<jlong>(angleChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_channels_OrientationChannel_readData(JNIEnv *env, jobject instance,
                                                           jlong offset, jlong length) {
    try {
        auto &angleChannelWrap = *extract_pointer<JniOrientationChannelWrap>(env, instance);
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

void JniOrientationChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    mListener = this->object->subscribeLengthChanged([weakReference](auto length){
        jni::call_in_attached_thread([&weakReference, &length](auto env){
            sendNotification<long>(env, weakReference, length);
        });
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