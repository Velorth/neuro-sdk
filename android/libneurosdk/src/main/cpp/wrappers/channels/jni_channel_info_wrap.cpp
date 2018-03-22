#include "wrappers/channels/jni_channel_info_wrap.h"

template<>
const std::map<Neuro::ChannelInfo::Type, std::string>
jni::enum_name_map<Neuro::ChannelInfo::Type>::mEnumToNameMap = []() {
    return std::map<Neuro::ChannelInfo::Type, std::string>{
            {Neuro::ChannelInfo::Type::Signal,          "Signal"},
            {Neuro::ChannelInfo::Type::Battery,         "Battery"},
            {Neuro::ChannelInfo::Type::ElectrodesState, "ElectrodesState"},
            {Neuro::ChannelInfo::Type::Respiration,     "Respiration"},
            {Neuro::ChannelInfo::Type::MEMS,            "MEMS"},
            {Neuro::ChannelInfo::Type::Angle,           "Angle"},
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
            {"Angle",           Neuro::ChannelInfo::Type::Angle},
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
        jni::java_object<Neuro::ChannelInfo::Type> typeObject(channelInfo.getType());
        auto channelType = env->NewLocalRef(typeObject);
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class, objectClassConstructor, channelType,channelName));
    });
}

