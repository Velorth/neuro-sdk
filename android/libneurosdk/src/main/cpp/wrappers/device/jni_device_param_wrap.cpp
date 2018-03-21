#include "wrappers/device/jni_device_param_wrap.h"
#include "wrappers/device/jni_param_types_wrap.h"

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
                throw std::runtime_error("Unresolved state enum value");
        }
    });
}

template<>
template<>
jni::java_object<Neuro::Command>::java_object(const Neuro::Command &cmd){
    call_in_attached_thread([=](auto env) {
        switch (cmd) {
            case Neuro::Command::StartSignal: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StartSignal"));
                break;
            }
            case Neuro::Command::StopSignal: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StopSignal"));
                break;
            }
            case Neuro::Command::StartResist: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StartResist"));
                break;
            }
            case Neuro::Command::StopResist: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StopResist"));
                break;
            }
            case Neuro::Command::StartMEMS: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StartMEMS"));
                break;
            }
            case Neuro::Command::StopMEMS: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StopMEMS"));
                break;
            }
            case Neuro::Command::StartRespiration: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StartRespiration"));
                break;
            }
            case Neuro::Command::StopRespiration: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StopRespiration"));
                break;
            }
            case Neuro::Command::StartStimulation: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "StartStimulation"));
                break;
            }
            case Neuro::Command::EnableMotionAssistant: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Command>(env, "EnableMotionAssistant"));
                break;
            }
            default:
                throw std::runtime_error("Unresolved command enum value");
        }
    });
}

template<>
template<>
jni::java_object<Neuro::Parameter>::java_object(const Neuro::Parameter &param){
    call_in_attached_thread([=](auto env) {
        switch (param) {
            case Neuro::Parameter::State: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "State"));
                break;
            }
            case Neuro::Parameter::Name: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "Name"));
                break;
            }
            case Neuro::Parameter::Address: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "Address"));
                break;
            }
            case Neuro::Parameter::SerialNumber: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "SerialNumber"));
                break;
            }
            case Neuro::Parameter::FirmwareMode: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "FirmwareMode"));
                break;
            }
            case Neuro::Parameter::SamplingFrequency: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "SamplingFrequency"));
                break;
            }
            case Neuro::Parameter::Gain: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "Gain"));
                break;
            }
            case Neuro::Parameter::Offset: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "Offset"));
                break;
            }
            case Neuro::Parameter::HardwareFilterState: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "HardwareFilterState"));
                break;
            }
            case Neuro::Parameter::ExternalSwitchState: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "ExternalSwitchState"));
                break;
            }
            case Neuro::Parameter::ADCInputState: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "ADCInputState"));
                break;
            }
            case Neuro::Parameter::StimulatorState: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "StimulatorState"));
                break;
            }
            case Neuro::Parameter::StimulatorParamPack: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "StimulatorParamPack"));
                break;
            }
            case Neuro::Parameter::MotionAssistantState: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "MotionAssistantState"));
                break;
            }
            case Neuro::Parameter::MotionAssistantParamPack: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::Parameter>(env, "MotionAssistantParamPack"));
                break;
            }
            default:
                throw std::runtime_error("Unresolved enum value");
        }
    });
}

template<>
template<>
jni::java_object<Neuro::ParamAccess>::java_object(const Neuro::ParamAccess &access){
    call_in_attached_thread([=](auto env) {
        switch (access) {
            case Neuro::ParamAccess::Read: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::DeviceState>(env, "Read"));
                break;
            }
            case Neuro::ParamAccess::ReadWrite: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::DeviceState>(env, "ReadWrite"));
                break;
            }
            case Neuro::ParamAccess::ReadNotify: {
                javaObj = make_global_ref_ptr(
                        getEnumFieldRef<Neuro::DeviceState>(env, "ReadNotify"));
                break;
            }
            default:
                throw std::runtime_error("Unresolved state enum value");
        }
    });
}

template<>
template<>
jni::java_object<Neuro::ParamPair>::java_object(const Neuro::ParamPair &param_pair){
    call_in_attached_thread([=](JNIEnv *env){
        auto paramName = env->NewLocalRef(java_object<Neuro::Parameter>(param_pair.first));
        auto paramAccess = env->NewLocalRef(java_object<Neuro::ParamAccess>(param_pair.second));
        auto paramType = env->NewLocalRef(java_object<ParameterType>(param_pair.first));
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::ParamPair>());
        javaObj = make_global_ref_ptr(env->NewObject(object_class,
                                                     objectClassConstructor,
                                                     paramName,
                                                     paramAccess,
                                                     paramType));
    });
}