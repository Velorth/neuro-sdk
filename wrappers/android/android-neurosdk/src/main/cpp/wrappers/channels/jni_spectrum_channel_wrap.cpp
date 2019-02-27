#include "java_helper.h"
#include "cspectrum-channel.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_createSpectrumDoubleChannel(JNIEnv *env, jclass, jlong channelPtr) {
    auto doubleChannel = reinterpret_cast<DoubleChannel *>(channelPtr);
    auto signalChannel = create_SpectrumDoubleChannel(doubleChannel);
    if (signalChannel == nullptr){
        char errorMsg[256];
        sdk_last_error_msg(errorMsg, 256);
        java_throw(env, errorMsg);
        return 0;
    }
    return reinterpret_cast<jlong>(signalChannel);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_SpectrumDoubleChannelGetHzPerSpectrumSample(JNIEnv *env, jclass, jlong spectrumChannelPtr) {
    auto spectrumChannel = reinterpret_cast<SpectrumDoubleChannel *>(spectrumChannelPtr);
    double hzPerSample;
    throw_if_error(env, SpectrumDoubleChannel_get_hz_per_spectrum_sample(spectrumChannel, &hzPerSample));
    return hzPerSample;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_SpectrumChannel_SpectrumDoubleChannelGetSpectrumLength(JNIEnv *env, jclass, jlong spectrumChannelPtr) {
    auto spectrumChannel = reinterpret_cast<SpectrumDoubleChannel *>(spectrumChannelPtr);
    size_t spectrumLength;
    throw_if_error(env, SpectrumDoubleChannel_get_spectrum_length(spectrumChannel, &spectrumLength));
    return spectrumLength;
}