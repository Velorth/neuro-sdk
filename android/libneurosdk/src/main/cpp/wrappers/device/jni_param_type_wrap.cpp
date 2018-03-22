#include "wrappers/device/jni_param_types_wrap.h"

std::string getParamTypeName(Neuro::Parameter param) {
    switch (param){
        case Neuro::Parameter::Name:
        case Neuro::Parameter::Address:
        case Neuro::Parameter::SerialNumber:
            return "String";
        case Neuro::Parameter::HardwareFilterState:
        case Neuro::Parameter::StimulatorState:
        case Neuro::Parameter::MotionAssistantState:
            return "Boolean";
        case Neuro::Parameter::Offset:
            return "Byte";
        case Neuro::Parameter::ExternalSwitchState:
            return "ExternalSwitchInput";
        case Neuro::Parameter::State:
            return "DeviceState";
        case Neuro::Parameter::FirmwareMode:
            return "FirmwareMode";
        case Neuro::Parameter::SamplingFrequency:
            return "SamplingFrequency";
        case Neuro::Parameter::Gain:
            return "Gain";
        case Neuro::Parameter::ADCInputState:
            return "ADCInput";
        case Neuro::Parameter::StimulatorParamPack:
            return "StimulationParams";
        case Neuro::Parameter::MotionAssistantParamPack:
            return "MotionAssistantParams";
    }
}

ParameterType::ParameterType(Neuro::Parameter param):
        mName(getParamTypeName(param)) {
}

std::string ParameterType::name() const {
    return mName;
}

template<>
template<>
jni::java_object<ParameterType>::java_object(const ParameterType &param_type):
    nativeObj(param_type){
    call_in_attached_thread([=](auto env){
        javaObj = make_global_ref_ptr(
                getEnumFieldRef<ParameterType>(env, param_type.name().c_str()));
    });
}
