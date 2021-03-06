#include <saturation_cast.h>
#include "wrappers/jni_ptr_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"

extern "C"
{

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_channels_NativeChannelInfo_getName(JNIEnv *env, jobject instance) {
    auto infoPtr = extract_pointer<Neuro::ChannelInfo>(env, instance);
    jobject nameObj = jni::java_object<std::string>(infoPtr->getName());
    return static_cast<jstring>(nameObj);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_channels_NativeChannelInfo_getType(JNIEnv *env, jobject instance) {
    auto infoPtr = extract_pointer<Neuro::ChannelInfo>(env, instance);
    return jni::java_object<Neuro::ChannelInfo::Type>(infoPtr->getType());;
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_channels_NativeChannelInfo_setName(JNIEnv *env, jobject instance,
                                                              jstring name_) {
    const char *name = env->GetStringUTFChars(name_, 0);

    auto infoPtr = extract_pointer<Neuro::ChannelInfo>(env, instance);
    infoPtr->setName(name);

    env->ReleaseStringUTFChars(name_, name);
}

JNIEXPORT jlong JNICALL
Java_com_neuromd_neurosdk_channels_NativeChannelInfo_getIndex(JNIEnv *env, jobject instance){
    auto infoPtr = extract_pointer<Neuro::ChannelInfo>(env, instance);
    return saturation_cast<jlong>(infoPtr->getIndex());
}

}

template<>
const std::map<Neuro::ChannelInfo::Type, std::string>
jni::enum_name_map<Neuro::ChannelInfo::Type>::mEnumToNameMap = []() {
    return std::map<Neuro::ChannelInfo::Type, std::string>{
            {Neuro::ChannelInfo::Type::Signal,          "Signal"},
            {Neuro::ChannelInfo::Type::Battery,         "Battery"},
            {Neuro::ChannelInfo::Type::ElectrodesState, "ElectrodesState"},
            {Neuro::ChannelInfo::Type::Respiration,     "Respiration"},
            {Neuro::ChannelInfo::Type::MEMS,            "MEMS"},
            {Neuro::ChannelInfo::Type::Orientation,     "Orientation"},
            {Neuro::ChannelInfo::Type::ConnectionStats, "ConnectionStats"},
            {Neuro::ChannelInfo::Type::Resistance,      "Resistance"},
            {Neuro::ChannelInfo::Type::Pedometer,       "Pedometer"},
            {Neuro::ChannelInfo::Type::Custom,          "Custom"}
    };
}();

template<>
const std::map<std::string, Neuro::ChannelInfo::Type>
jni::enum_name_map<Neuro::ChannelInfo::Type>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::ChannelInfo::Type>{
            {"Signal",          Neuro::ChannelInfo::Type::Signal},
            {"Battery",         Neuro::ChannelInfo::Type::Battery},
            {"ElectrodesState", Neuro::ChannelInfo::Type::ElectrodesState},
            {"Respiration",     Neuro::ChannelInfo::Type::Respiration},
            {"MEMS",            Neuro::ChannelInfo::Type::MEMS},
            {"Orientation",     Neuro::ChannelInfo::Type::Orientation},
            {"ConnectionStats", Neuro::ChannelInfo::Type::ConnectionStats},
            {"Resistance",      Neuro::ChannelInfo::Type::Resistance},
            {"Pedometer",       Neuro::ChannelInfo::Type::Pedometer},
            {"Custom",          Neuro::ChannelInfo::Type::Custom}
    };
}();

template<>
template<>
jni::java_object<Neuro::ChannelInfo>::java_object(const Neuro::ChannelInfo &channelInfo):
        nativeObj(channelInfo){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::ChannelInfo>());
        auto channelName = env->NewStringUTF(channelInfo.getName().c_str());
        auto channelType = static_cast<jobject>(jni::java_object<Neuro::ChannelInfo::Type>(channelInfo.getType()));
        auto index = static_cast<jlong>(channelInfo.getIndex());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class, objectClassConstructor, channelType, channelName, index));
    });
}

