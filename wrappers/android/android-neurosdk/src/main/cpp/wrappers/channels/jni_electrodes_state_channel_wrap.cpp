#include "saturation_cast.h"
#include "wrappers/channels/jni_channel_factory.h"
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/channels/jni_electrodes_state_channel_wrap.h"
#include "logger.h"

extern "C"
{

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_create(JNIEnv *env, jclass type,
                                                          jobject device) {
    return createChannelFromDevice<JniElectrodesStateChannelWrap>(env, device);
}

JNIEXPORT jobject
JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_info(JNIEnv *env, jobject instance) {

    auto &electrodesStateChannelWrap = *extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
    auto channelInfo = &electrodesStateChannelWrap->info();
    return jni::java_object<decltype(channelInfo)>(channelInfo);
}

JNIEXPORT void
JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_init(JNIEnv *env, jobject instance) {

    auto electrodesStateChannelWrap = extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
    electrodesStateChannelWrap->subscribeLengthChanged(
            find_notifier<decltype(electrodesStateChannelWrap)>(instance, "dataLengthChanged"));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_deleteNative(JNIEnv *env, jobject instance) {
    deleteNativeObject<JniElectrodesStateChannelWrap>(env, instance);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_underlyingDevice(JNIEnv *env, jobject instance) {
    auto &electrodesStateChannelWrap = *extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
    auto devicePtr = electrodesStateChannelWrap->underlyingDevice().lock();
    if (!devicePtr){
        return nullptr;
    }
    auto deviceWrap = new JniDeviceWrap(devicePtr);
    return jni::java_object<decltype(deviceWrap)>(deviceWrap);;
}

JNIEXPORT jfloat JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_samplingFrequency(JNIEnv *env,
                                                                     jobject instance) {
    auto &electrodesStateChannelWrap = *extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
    return electrodesStateChannelWrap->samplingFrequency();
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_bufferSize(JNIEnv *env, jobject instance) {
    auto &electrodesStateChannelWrap = *extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
    return saturation_cast<jlong>(electrodesStateChannelWrap->bufferSize());
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_totalLength(JNIEnv *env, jobject instance) {
    auto &electrodesStateChannelWrap = *extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
    return saturation_cast<jlong>(electrodesStateChannelWrap->totalLength());
}

JNIEXPORT jobjectArray JNICALL
Java_com_neuromd_neurosdk_channels_ElectrodesStateChannel_readData(JNIEnv *env, jobject instance,
                                                            jlong offset, jlong length) {
    try {
        auto &electrodesStateChannelWrap = *extract_pointer<JniElectrodesStateChannelWrap>(env, instance);
        auto data = electrodesStateChannelWrap->readData(saturation_cast<Neuro::data_offset_t>(offset),
                                                 saturation_cast<Neuro::data_length_t>(length));
        return jni::to_obj_array(env, data);
    }
    catch (std::exception &e){
        jni::java_throw(env, "UnsupportedOperationException", e);
        return nullptr;
    }
}

}

void JniElectrodesStateChannelWrap::subscribeLengthChanged(jobject stateChangedSubscriberRef) {
    lengthChangedGlobalSubscriberRef = jni::make_global_ref_ptr(stateChangedSubscriberRef);
    std::weak_ptr<jni::jobject_t> weakReference = lengthChangedGlobalSubscriberRef;
    mListener = this->object->subscribeLengthChanged([weakReference](auto length){
        jni::call_in_attached_thread([&weakReference, &length](auto env){
            sendNotification<long>(env, weakReference, length);
        });
    });
}

template<>
const std::map<Neuro::ElectrodeState, std::string>
        jni::enum_name_map<Neuro::ElectrodeState>::mEnumToNameMap = []() {
    return std::map<Neuro::ElectrodeState, std::string>{
            {Neuro::ElectrodeState::Normal,         "Normal"},
            {Neuro::ElectrodeState::HighResistance, "HighResistance"},
            {Neuro::ElectrodeState::Detached,       "Detached"}
    };
}();

template<>
const std::map<std::string, Neuro::ElectrodeState>
        jni::enum_name_map<Neuro::ElectrodeState>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::ElectrodeState>{
            {"Normal",         Neuro::ElectrodeState::Normal},
            {"HighResistance", Neuro::ElectrodeState::HighResistance},
            {"Detached",       Neuro::ElectrodeState::Detached}
    };
}();

