#include "wrappers/device/jni_param_types_wrap.h"

template<>
const std::map<Neuro::ADCInput, std::string>
        jni::enum_name_map<Neuro::ADCInput>::mEnumToNameMap = []() {
    return std::map<Neuro::ADCInput, std::string>{
            {Neuro::ADCInput::Resistance, "Resistance"},
            {Neuro::ADCInput::Electrodes, "Electrodes"},
            {Neuro::ADCInput::Short,      "Short"},
            {Neuro::ADCInput::Test,       "Test"}
    };
}();

template<>
const std::map<std::string, Neuro::ADCInput>
        jni::enum_name_map<Neuro::ADCInput>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::ADCInput>{
            {"Resistance", Neuro::ADCInput::Resistance},
            {"Electrodes", Neuro::ADCInput::Electrodes},
            {"Short",      Neuro::ADCInput::Short},
            {"Test",       Neuro::ADCInput::Test}
    };
}();

template<>
const std::map<Neuro::ExternalSwitchInput, std::string>
        jni::enum_name_map<Neuro::ExternalSwitchInput>::mEnumToNameMap = []() {
    return std::map<Neuro::ExternalSwitchInput, std::string>{
            {Neuro::ExternalSwitchInput::MioElectrodesRespUSB, "MioElectrodesRespUSB"},
            {Neuro::ExternalSwitchInput::MioElectrodes,        "MioElectrodes"},
            {Neuro::ExternalSwitchInput::MioUSB,               "MioUSB"},
            {Neuro::ExternalSwitchInput::RespUSB,              "RespUSB"}
    };
}();

template<>
const std::map<std::string, Neuro::ExternalSwitchInput>
        jni::enum_name_map<Neuro::ExternalSwitchInput>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::ExternalSwitchInput>{
            {"MioElectrodesRespUSB", Neuro::ExternalSwitchInput::MioElectrodesRespUSB},
            {"MioElectrodes",        Neuro::ExternalSwitchInput::MioElectrodes},
            {"MioUSB",               Neuro::ExternalSwitchInput::MioUSB},
            {"RespUSB",              Neuro::ExternalSwitchInput::RespUSB}
    };
}();

template<>
const std::map<Neuro::FirmwareMode, std::string>
        jni::enum_name_map<Neuro::FirmwareMode>::mEnumToNameMap = []() {
    return std::map<Neuro::FirmwareMode, std::string>{
            {Neuro::FirmwareMode::Application, "Application"},
            {Neuro::FirmwareMode::Bootloader,  "Bootloader"}
    };
}();

template<>
const std::map<std::string, Neuro::FirmwareMode>
        jni::enum_name_map<Neuro::FirmwareMode>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::FirmwareMode>{
            {"Application", Neuro::FirmwareMode::Application},
            {"Bootloader",  Neuro::FirmwareMode::Bootloader}
    };
}();

template<>
const std::map<Neuro::Gain, std::string>
        jni::enum_name_map<Neuro::Gain>::mEnumToNameMap = []() {
    return std::map<Neuro::Gain, std::string>{
            {Neuro::Gain::Gain1, "Gain1"},
            {Neuro::Gain::Gain2, "Gain2"},
            {Neuro::Gain::Gain3, "Gain3"},
            {Neuro::Gain::Gain4, "Gain4"},
            {Neuro::Gain::Gain6, "Gain6"},
            {Neuro::Gain::Gain8, "Gain8"},
            {Neuro::Gain::Gain12, "Gain12"}
    };
}();

template<>
const std::map<std::string, Neuro::Gain>
        jni::enum_name_map<Neuro::Gain>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::Gain>{
            {"Gain1", Neuro::Gain::Gain1},
            {"Gain2", Neuro::Gain::Gain2},
            {"Gain3", Neuro::Gain::Gain3},
            {"Gain4", Neuro::Gain::Gain4},
            {"Gain6", Neuro::Gain::Gain6},
            {"Gain8", Neuro::Gain::Gain8},
            {"Gain12", Neuro::Gain::Gain12}
    };
}();

template<>
const std::map<Neuro::MotionAssistantLimb, std::string>
        jni::enum_name_map<Neuro::MotionAssistantLimb>::mEnumToNameMap = []() {
    return std::map<Neuro::MotionAssistantLimb, std::string>{
            {Neuro::MotionAssistantLimb::RightLeg, "RightLeg"},
            {Neuro::MotionAssistantLimb::LeftLeg,  "LeftLeg"},
            {Neuro::MotionAssistantLimb::RightArm, "RightArm"},
            {Neuro::MotionAssistantLimb::LeftArm,  "LeftArm"}
    };
}();

template<>
const std::map<std::string, Neuro::MotionAssistantLimb>
        jni::enum_name_map<Neuro::MotionAssistantLimb>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::MotionAssistantLimb>{
            {"RightLeg", Neuro::MotionAssistantLimb::RightLeg},
            {"LeftLeg", Neuro::MotionAssistantLimb::LeftLeg},
            {"RightArm", Neuro::MotionAssistantLimb::RightArm},
            {"LeftArm", Neuro::MotionAssistantLimb::LeftArm}
    };
}();

template<>
const std::map<Neuro::SamplingFrequency, std::string>
        jni::enum_name_map<Neuro::SamplingFrequency>::mEnumToNameMap = []() {
    return std::map<Neuro::SamplingFrequency, std::string>{
            {Neuro::SamplingFrequency::Hz125,  "Hz125"},
            {Neuro::SamplingFrequency::Hz250,  "Hz250"},
            {Neuro::SamplingFrequency::Hz500,  "Hz500"},
            {Neuro::SamplingFrequency::Hz1000, "Hz1000"},
            {Neuro::SamplingFrequency::Hz2000, "Hz2000"},
            {Neuro::SamplingFrequency::Hz4000, "Hz4000"},
            {Neuro::SamplingFrequency::Hz8000, "Hz8000"}
    };
}();

template<>
const std::map<std::string, Neuro::SamplingFrequency>
        jni::enum_name_map<Neuro::SamplingFrequency>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::SamplingFrequency>{
            {"Hz125", Neuro::SamplingFrequency::Hz125},
            {"Hz250", Neuro::SamplingFrequency::Hz250},
            {"Hz500", Neuro::SamplingFrequency::Hz500},
            {"Hz1000", Neuro::SamplingFrequency::Hz1000},
            {"Hz2000", Neuro::SamplingFrequency::Hz2000},
            {"Hz4000", Neuro::SamplingFrequency::Hz4000},
            {"Hz8000", Neuro::SamplingFrequency::Hz8000}
    };
}();

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
