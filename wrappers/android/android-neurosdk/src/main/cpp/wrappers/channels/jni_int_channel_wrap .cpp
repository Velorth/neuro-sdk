#include <vector>
#include "java_helper.h"


extern "C"
JNIEXPORT jintArray JNICALL
Java_com_neuromd_neurosdk_channels_IntDataChannel_IntChannelReadData(JNIEnv *env, jclass, jlong intChannelPtr, jlong offset, jlong length) {
    auto intChannel = reinterpret_cast<IntChannel *>(intChannelPtr);
    if (offset < 0 || length < 0){
        java_throw(env, "Invalid offset or length");
        return nullptr;
    }

    std::vector<int> intData(static_cast<size_t>(length));
    auto result = IntChannel_read_data(intChannel, static_cast<size_t>(offset), static_cast<size_t>(length), intData.data());
    if (result != SDK_NO_ERROR){
        throw_if_error(env, result);
        return nullptr;
    }

    try {
        return java_array_from_IntDataArray(env, intData.data(), intData.size());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}