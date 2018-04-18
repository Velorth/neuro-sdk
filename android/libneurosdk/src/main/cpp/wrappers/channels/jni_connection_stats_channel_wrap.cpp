#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/channels/jni_connection_stats_channel_wrap.h"
#include "channels/connection_stats_channel.h"


extern "C"
{

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_create(JNIEnv *env, jclass type,
                                                          jobject device) {
    return createChannelFromDevice<JniConnectionStatsChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_info(JNIEnv *env, jobject instance) {

    auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    auto channelInfo = &connectionStatsChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_init(JNIEnv *env, jobject instance) {

    auto connectionStatsChannelWrap = extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    connectionStatsChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(connectionStatsChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniConnectionStatsChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    auto devicePtr = connectionStatsChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);;
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_setSamplingFrequency(JNIEnv *env,
                                                                        jobject instance,
                                                                        jfloat frequency) {
    auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    try {
        connectionStatsChannelWrap->setSamplingFrequency(frequency);
    }
    catch (std::runtime_error &e) {
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return;
    }
}

JNIEXPORT jfloat JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_samplingFrequency(JNIEnv *env,
                                                                     jobject instance) {
    auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    return connectionStatsChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    return saturation_cast<jlong>(connectionStatsChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
    return saturation_cast<jlong>(connectionStatsChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_ru_neurotech_neurosdk_channels_ConnectionStatsChannel_readData(JNIEnv *env, jobject instance,
                                                            jlong offset, jlong length) {
    try {
        auto &connectionStatsChannelWrap = *extract_pointer<JniConnectionStatsChannelWrap>(env, instance);
        auto data = connectionStatsChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                 saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

}

void JniConnectionStatsChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    mListener = this->object->subscribeLengthChanged([weakReference](auto length){
        jni::call_in_attached_thread([&weakReference, &length](auto env){
            sendNotification<long>(env, weakReference, length);
        });
    });
}

