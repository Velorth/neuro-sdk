#include "wrappers/jni_device_param_wrap.h"

template<>
template<>
jni::java_object<Neuro::DeviceState>::java_object(const Neuro::DeviceState &state){
    call_in_attached_thread([=](auto env) {
        switch (state) {
            case Neuro::DeviceState::Connected: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::DeviceState>(env, "Connected"));
                break;
            }
            case Neuro::DeviceState::Disconnected: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::DeviceState>(env, "Disconnected"));
                break;
            }
            default:
                throw std::runtime_error("Unresolved enum value");
        }
    });
}

template<>
template<>
jni::java_object<Neuro::Command>::java_object(const Neuro::Command &){

}

template<>
template<>
jni::java_object<Neuro::Parameter>::java_object(const Neuro::Parameter &){

}

template<>
template<>
jni::java_object<Neuro::ParamAccess>::java_object(const Neuro::ParamAccess &){

}