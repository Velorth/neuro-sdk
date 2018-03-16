#include "wrappers/channels/jni_base_channel_wrap.h"

extern "C"
{

JNIEXPORT jobject
JNICALL
Java_ru_neurotech_neurosdk_channels_NativeChannel_info(JNIEnv *env, jobject instance) {

    auto baseChannelPtr = *extract_pointer<JniBaseChannelWrap>(env, instance);
    auto channelInfo = &baseChannelPtr->info();
}

JNIEXPORT void
JNICALL
Java_ru_neurotech_neurosdk_channels_NativeChannel_init(JNIEnv *env, jobject instance) {

    auto baseChannelPtr = extract_pointer<JniBaseChannelWrap>(env, instance);
    baseChannelPtr->subscribeLengthChanged(
            find_notifier<decltype(baseChannelPtr)>(instance, "dataLengthChanged"));
}

}

void JniBaseChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    this->object->setLengthChangedCallback([weakReference](auto length){
        sendNotification<long>(weakReference, length);
    });
}
