#include <vector>
#include "java_helper.h"


extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_neuromd_neurosdk_channels_DoubleDataChannel_DoubleChannelReadData(JNIEnv *env, jclass, jlong doubleChannelPtr, jlong offset, jlong length) {
    auto doubleChannel = reinterpret_cast<DoubleChannel *>(doubleChannelPtr);
    if (offset < 0 || length < 0){
        java_throw(env, "Invalid offset or length");
        return nullptr;
    }

    std::vector<double> doubleData(static_cast<size_t>(length));
    auto result = DoubleChannel_read_data(doubleChannel, static_cast<size_t>(offset), static_cast<size_t>(length), doubleData.data());
    if (result != SDK_NO_ERROR){
        throw_if_error(env, result);
        return nullptr;
    }

    try {
        return java_array_from_DoubleDataArray(env, doubleData.data(), doubleData.size());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}