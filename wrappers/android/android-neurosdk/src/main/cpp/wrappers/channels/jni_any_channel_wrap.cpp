#include "java_helper.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_freeListenerHandle(JNIEnv *env, jclass type,
                                                                 jlong listenerHandle) {

    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelDelete(JNIEnv *env, jclass type,
                                                               jlong anyChannelPtr) {

    // TODO

}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelGetInfo(JNIEnv *env, jclass type,
                                                                jlong anyChannelPtr) {

    // TODO

}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelGetSamplingFrequency(JNIEnv *env,
                                                                             jclass type,
                                                                             jlong anyChannelPtr) {

    // TODO

}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelSubscribeLengthChanged(JNIEnv *env,
                                                                               jobject instance,
                                                                               jlong anyChannelPtr) {

    // TODO

}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_AnyChannel_AnyChannelGetTotalLength(JNIEnv *env, jclass type,
                                                                       jlong anyChannelPtr) {

    // TODO

}