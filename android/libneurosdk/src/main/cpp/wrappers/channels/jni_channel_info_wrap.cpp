#include "wrappers/channels/jni_channel_info_wrap.h"


template<>
template<>
jni::java_object<Neuro::ChannelInfo::Type>::java_object(const Neuro::ChannelInfo::Type &type) {
    call_in_attached_thread([=](auto env) {
        switch (type) {
            case Neuro::ChannelInfo::Type::Signal: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Signal"));
                break;
            }
            case Neuro::ChannelInfo::Type::Battery: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Battery"));
                break;
            }
            case Neuro::ChannelInfo::Type::ElectrodesState: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "ElectrodesState"));
                break;
            }
            case Neuro::ChannelInfo::Type::Respiration: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Respiration"));
                break;
            }
            case Neuro::ChannelInfo::Type::MEMS: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "MEMS"));
                break;
            }
            case Neuro::ChannelInfo::Type::Angle: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Angle"));
                break;
            }
            case Neuro::ChannelInfo::Type::ConnectionStats: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "ConnectionStats"));
                break;
            }
            case Neuro::ChannelInfo::Type::Resistance: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Resistance"));
                break;
            }
            case Neuro::ChannelInfo::Type::Pedometer: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Pedometer"));
                break;
            }
            case Neuro::ChannelInfo::Type::Custom: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::ChannelInfo::Type>(env, "Custom"));
                break;
            }
            default:
                throw std::runtime_error("Unresolved state enum value");
        }
    });
}

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

