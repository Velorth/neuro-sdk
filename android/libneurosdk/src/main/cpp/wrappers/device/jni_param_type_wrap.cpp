#include "wrappers/device/jni_param_types_wrap.h"

std::string getParamTypeName(Neuro::Parameter param) {
    switch (param){
        case Neuro::Parameter::Name:
        case Neuro::Parameter::Address:
        case Neuro::Parameter::SerialNumber:
            return "java/lang/String";
        case Neuro::Parameter::HardwareFilterState:
        case Neuro::Parameter::StimulatorState:
        case Neuro::Parameter::MotionAssistantState:
            return "java/lang/Boolean";
        case Neuro::Parameter::Offset:
            return "java/lang/Byte";
        case Neuro::Parameter::ExternalSwitchState:
            return "ru/neurotech/neurosdk/parameters/types/ExternalSwitchInput";
        case Neuro::Parameter::State:
            return "ru/neurotech/neurosdk/parameters/types/DeviceState";
        case Neuro::Parameter::FirmwareMode:
            return "ru/neurotech/neurosdk/parameters/types/FirmwareMode";
        case Neuro::Parameter::SamplingFrequency:
            return "ru/neurotech/neurosdk/parameters/types/SamplingFrequency";
        case Neuro::Parameter::Gain:
            return "ru/neurotech/neurosdk/parameters/types/Gain";
        case Neuro::Parameter::ADCInputState:
            return "ru/neurotech/neurosdk/parameters/types/ADCInput";
        case Neuro::Parameter::StimulatorParamPack:
            return "ru/neurotech/neurosdk/parameters/types/StimulationParams";
        case Neuro::Parameter::MotionAssistantParamPack:
            return "ru/neurotech/neurosdk/parameters/types/MotionAssistantParams";
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

    });
}
