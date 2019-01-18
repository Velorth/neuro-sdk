#include "wrappers/device/jni_device_param_wrap.h"
#include "wrappers/device/jni_param_types_wrap.h"
#include "device/param_values.h"

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
            {Neuro::Command::StopStimulation,       "StopStimulation"},
            {Neuro::Command::EnableMotionAssistant, "EnableMotionAssistant"},
            {Neuro::Command::DisableMotionAssistant,"DisableMotionAssistant"},
            {Neuro::Command::FindMe,                "FindMe"}
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
            {"StopStimulation",       Neuro::Command::StopStimulation},
            {"EnableMotionAssistant", Neuro::Command::EnableMotionAssistant},
            {"DisableMotionAssistant",Neuro::Command::DisableMotionAssistant},
            {"FindMe",                Neuro::Command::FindMe}
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
            {Neuro::Parameter::GyroscopeSens,            "GyroscopeSens"},
            {Neuro::Parameter::AccelerometerSens,        "AccelerometerSens"},
            {Neuro::Parameter::StimulatorAndMAState,     "StimulatorAndMAState"},
            {Neuro::Parameter::StimulatorParamPack,      "StimulatorParamPack"},
            {Neuro::Parameter::MotionAssistantParamPack, "MotionAssistantParamPack"},
            {Neuro::Parameter::FirmwareVersion, "FirmwareVersion"}
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
            {"GyroscopeSens",            Neuro::Parameter::GyroscopeSens},
            {"AccelerometerSens",        Neuro::Parameter::AccelerometerSens},
            {"StimulatorAndMAState",     Neuro::Parameter::StimulatorAndMAState},
            {"StimulatorParamPack",      Neuro::Parameter::StimulatorParamPack},
            {"MotionAssistantParamPack", Neuro::Parameter::MotionAssistantParamPack},
            {"FirmwareVersion", Neuro::Parameter::FirmwareVersion}
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
        auto paramName  = static_cast<jobject>(java_object<Neuro::Parameter>(param_pair.first));
        auto paramAccess = static_cast<jobject>(java_object<Neuro::ParamAccess>(param_pair.second));
        auto paramType = static_cast<jobject>(java_object<ParameterType>(param_pair.first));
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::ParamPair>());
        javaObj = make_global_ref_ptr(env->NewObject(object_class,
                                                     objectClassConstructor,
                                                     paramName,
                                                     paramAccess,
                                                     paramType));
        env->DeleteLocalRef(paramName);
        env->DeleteLocalRef(paramAccess);
        env->DeleteLocalRef(paramType);
    });
}

template <Neuro::Parameter Param>
static jobject readParam(JNIEnv *env, const Neuro::Device &device){
    auto paramValue = device.readParam<Param>();
    return jni::java_object<typename Neuro::ParamValue<Param>::Type>(paramValue);
}

jobject readDeviceParam(JNIEnv *env, const Neuro::Device &device, Neuro::Parameter parameter) {
    try {
        switch (parameter) {
            case Neuro::Parameter::State: {
                return readParam<Neuro::Parameter::State>(env, device);
            }
            case Neuro::Parameter::Name: {
                return readParam<Neuro::Parameter::Name>(env, device);
            }
            case Neuro::Parameter::Address: {
                return readParam<Neuro::Parameter::Address>(env, device);
            }
            case Neuro::Parameter::SerialNumber: {
                return readParam<Neuro::Parameter::SerialNumber>(env, device);
            }
            case Neuro::Parameter::FirmwareMode: {
                return readParam<Neuro::Parameter::FirmwareMode>(env, device);
            }
            case Neuro::Parameter::SamplingFrequency: {
                return readParam<Neuro::Parameter::SamplingFrequency>(env, device);
            }
            case Neuro::Parameter::Gain: {
                return readParam<Neuro::Parameter::Gain>(env, device);
            }
            case Neuro::Parameter::Offset: {
                return readParam<Neuro::Parameter::Offset>(env, device);
            }
            case Neuro::Parameter::HardwareFilterState: {
                return readParam<Neuro::Parameter::HardwareFilterState>(env, device);
            }
            case Neuro::Parameter::ExternalSwitchState: {
                return readParam<Neuro::Parameter::ExternalSwitchState>(env, device);
            }
            case Neuro::Parameter::ADCInputState: {
                return readParam<Neuro::Parameter::ADCInputState>(env, device);
            }
            case Neuro::Parameter::AccelerometerSens: {
                return readParam<Neuro::Parameter::AccelerometerSens>(env, device);
            }
            case Neuro::Parameter::GyroscopeSens: {
                return readParam<Neuro::Parameter::GyroscopeSens>(env, device);
            }
            case Neuro::Parameter::StimulatorAndMAState: {
                return readParam<Neuro::Parameter::StimulatorAndMAState>(env, device);
            }
            case Neuro::Parameter::StimulatorParamPack: {
                return readParam<Neuro::Parameter::StimulatorParamPack>(env, device);
            }
            case Neuro::Parameter::MotionAssistantParamPack: {
                return readParam<Neuro::Parameter::MotionAssistantParamPack>(env, device);
            }
            case Neuro::Parameter::FirmwareVersion: {
                return readParam<Neuro::Parameter::FirmwareVersion>(env, device);
            }
            default: {
                jni::java_throw(env,
                                "java/lang/UnsupportedOperationException",
                                std::runtime_error("Parameter not found"));
                return nullptr;
            }
        }
    }
    catch (std::runtime_error &e){
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return nullptr;
    }
    catch (...){
        jni::java_throw(env, "java/lang/UnsupportedOperationException");
        return nullptr;
    }
}

template <Neuro::Parameter Param>
static
jboolean
setParam(JNIEnv *env, Neuro::Device &device, jobject valueObj){
    try {
        using param_value_t = typename Neuro::ParamValue<Param>::Type;
        auto value = jni::get_java_obj_value<param_value_t>(env, valueObj);
        return static_cast<jboolean>(device.setParam<Param>(value));
    }
    catch (std::runtime_error &e){
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return JNI_FALSE;
    }
}

jboolean
setDeviceParam(JNIEnv *env, Neuro::Device &device, Neuro::Parameter param, jobject valueObj){
    try {
        switch (param) {
            case Neuro::Parameter::State: {
                return setParam<Neuro::Parameter::State>(env, device, valueObj);
            }
            case Neuro::Parameter::Name: {
                return setParam<Neuro::Parameter::Name>(env, device, valueObj);
            }
            case Neuro::Parameter::Address: {
                return setParam<Neuro::Parameter::Address>(env, device, valueObj);
            }
            case Neuro::Parameter::SerialNumber: {
                return setParam<Neuro::Parameter::SerialNumber>(env, device, valueObj);
            }
            case Neuro::Parameter::FirmwareMode: {
                return setParam<Neuro::Parameter::FirmwareMode>(env, device, valueObj);
            }
            case Neuro::Parameter::SamplingFrequency: {
                return setParam<Neuro::Parameter::SamplingFrequency>(env, device, valueObj);
            }
            case Neuro::Parameter::Gain: {
                return setParam<Neuro::Parameter::Gain>(env, device, valueObj);
            }
            case Neuro::Parameter::Offset: {
                return setParam<Neuro::Parameter::Offset>(env, device, valueObj);
            }
            case Neuro::Parameter::HardwareFilterState: {
                return setParam<Neuro::Parameter::HardwareFilterState>(env, device, valueObj);
            }
            case Neuro::Parameter::ExternalSwitchState: {
                return setParam<Neuro::Parameter::ExternalSwitchState>(env, device, valueObj);
            }
            case Neuro::Parameter::ADCInputState: {
                return setParam<Neuro::Parameter::ADCInputState>(env, device, valueObj);
            }
            case Neuro::Parameter::GyroscopeSens: {
                return setParam<Neuro::Parameter::GyroscopeSens>(env, device, valueObj);
            }
            case Neuro::Parameter::AccelerometerSens: {
                return setParam<Neuro::Parameter::AccelerometerSens>(env, device, valueObj);
            }
            case Neuro::Parameter::StimulatorParamPack: {
                return setParam<Neuro::Parameter::StimulatorParamPack>(env, device, valueObj);
            }
            case Neuro::Parameter::MotionAssistantParamPack: {
                return setParam<Neuro::Parameter::MotionAssistantParamPack>(env, device, valueObj);
            }
            case Neuro::Parameter::FirmwareVersion: {
                return setParam<Neuro::Parameter::FirmwareVersion>(env, device, valueObj);
            }
            default: {
                jni::java_throw(env,
                                "java/lang/UnsupportedOperationException",
                                std::runtime_error("Parameter not found"));
                return JNI_FALSE;
            }
        }
    }
    catch (std::runtime_error &e){
        jni::java_throw(env, "java/lang/UnsupportedOperationException", e);
        return JNI_FALSE;
    }
}

