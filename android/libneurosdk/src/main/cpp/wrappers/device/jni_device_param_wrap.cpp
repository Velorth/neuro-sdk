#include "wrappers/device/jni_device_param_wrap.h"
#include "wrappers/device/jni_param_types_wrap.h"

template<>
const std::map<Neuro::DeviceState, std::string>
jni::enum_name_map<Neuro::DeviceState>::mEnumToNameMap = []() {
    return std::map<Neuro::DeviceState, std::string>{
            {Neuro::DeviceState::Connected,    "Connected"},
            {Neuro::DeviceState::Disconnected, "Disconnected"}
    };
}();

template<>
const std::map<std::string, Neuro::DeviceState>
jni::enum_name_map<Neuro::DeviceState>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::DeviceState>{
            {"Connected",    Neuro::DeviceState::Connected},
            {"Disconnected", Neuro::DeviceState::Disconnected}
    };
}();

template<>
const std::map<Neuro::Command, std::string>
jni::enum_name_map<Neuro::Command>::mEnumToNameMap = []() {
    return std::map<Neuro::Command, std::string>{
            {Neuro::Command::StartSignal,           "StartSignal"},
            {Neuro::Command::StopSignal,            "StopSignal"},
            {Neuro::Command::StartResist,           "StartResist"},
            {Neuro::Command::StopResist,            "StopResist"},
            {Neuro::Command::StartMEMS,             "StartMEMS"},
            {Neuro::Command::StopMEMS,              "StopMEMS"},
            {Neuro::Command::StartRespiration,      "StartRespiration"},
            {Neuro::Command::StopRespiration,       "StopRespiration"},
            {Neuro::Command::StartStimulation,      "StartStimulation"},
            {Neuro::Command::EnableMotionAssistant, "EnableMotionAssistant"}
    };
}();

template<>
const std::map<std::string, Neuro::Command>
jni::enum_name_map<Neuro::Command>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::Command>{
            {"StartSignal",           Neuro::Command::StartSignal},
            {"StopSignal",            Neuro::Command::StopSignal},
            {"StartResist",           Neuro::Command::StartResist},
            {"StopResist",            Neuro::Command::StopResist},
            {"StartMEMS",             Neuro::Command::StartMEMS},
            {"StopMEMS",              Neuro::Command::StopMEMS},
            {"StartRespiration",      Neuro::Command::StartRespiration},
            {"StopRespiration",       Neuro::Command::StopRespiration},
            {"StartStimulation",      Neuro::Command::StartStimulation},
            {"EnableMotionAssistant", Neuro::Command::EnableMotionAssistant}
    };
}();

template<>
const std::map<Neuro::Parameter, std::string>
jni::enum_name_map<Neuro::Parameter>::mEnumToNameMap = []() {
    return std::map<Neuro::Parameter, std::string>{
            {Neuro::Parameter::State,                    "State"},
            {Neuro::Parameter::Name,                     "Name"},
            {Neuro::Parameter::Address,                  "Address"},
            {Neuro::Parameter::SerialNumber,             "SerialNumber"},
            {Neuro::Parameter::FirmwareMode,             "FirmwareMode"},
            {Neuro::Parameter::SamplingFrequency,        "SamplingFrequency"},
            {Neuro::Parameter::Gain,                     "Gain"},
            {Neuro::Parameter::Offset,                   "Offset"},
            {Neuro::Parameter::HardwareFilterState,      "HardwareFilterState"},
            {Neuro::Parameter::ExternalSwitchState,      "ExternalSwitchState"},
            {Neuro::Parameter::ADCInputState,            "ADCInputState"},
            {Neuro::Parameter::StimulatorState,          "StimulatorState"},
            {Neuro::Parameter::StimulatorParamPack,      "StimulatorParamPack"},
            {Neuro::Parameter::MotionAssistantState,     "MotionAssistantState"},
            {Neuro::Parameter::MotionAssistantParamPack, "MotionAssistantParamPack"}
    };
}();

template<>
const std::map<std::string, Neuro::Parameter>
jni::enum_name_map<Neuro::Parameter>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::Parameter>{
            {"State",                    Neuro::Parameter::State},
            {"Name",                     Neuro::Parameter::Name},
            {"Address",                  Neuro::Parameter::Address},
            {"SerialNumber",             Neuro::Parameter::SerialNumber},
            {"FirmwareMode",             Neuro::Parameter::FirmwareMode},
            {"SamplingFrequency",        Neuro::Parameter::SamplingFrequency},
            {"Gain",                     Neuro::Parameter::Gain},
            {"Offset",                   Neuro::Parameter::Offset},
            {"HardwareFilterState",      Neuro::Parameter::HardwareFilterState},
            {"ExternalSwitchState",      Neuro::Parameter::ExternalSwitchState},
            {"ADCInputState",            Neuro::Parameter::ADCInputState},
            {"StimulatorState",          Neuro::Parameter::StimulatorState},
            {"StimulatorParamPack",      Neuro::Parameter::StimulatorParamPack},
            {"MotionAssistantState",     Neuro::Parameter::MotionAssistantState},
            {"MotionAssistantParamPack", Neuro::Parameter::MotionAssistantParamPack}
    };
}();

template<>
const std::map<Neuro::ParamAccess, std::string>
jni::enum_name_map<Neuro::ParamAccess>::mEnumToNameMap = []() {
    return std::map<Neuro::ParamAccess, std::string>{
            {Neuro::ParamAccess::Read,       "Read"},
            {Neuro::ParamAccess::ReadWrite,  "ReadWrite"},
            {Neuro::ParamAccess::ReadNotify, "ReadNotify"}
    };
}();

template<>
const std::map<std::string, Neuro::ParamAccess>
jni::enum_name_map<Neuro::ParamAccess>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::ParamAccess>{
            {"Read",       Neuro::ParamAccess::Read},
            {"ReadWrite",  Neuro::ParamAccess::ReadWrite},
            {"ReadNotify", Neuro::ParamAccess::ReadNotify}
    };
}();

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

