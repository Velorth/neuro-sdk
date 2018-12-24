#include <saturation_cast.h>
#include "wrappers/device/jni_param_types_wrap.h"

template<>
template<>
jni::java_object<Neuro::StimulationParams>::java_object(const Neuro::StimulationParams &params):
        nativeObj(params){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::StimulationParams>());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class,
                               objectClassConstructor,
                               params.current,
                               params.pulse_width,
                               params.frequency,
                               params.stimulus_duration));
    });
}

template <>
Neuro::StimulationParams jni::get_java_obj_value<Neuro::StimulationParams>(JNIEnv *env, jobject obj){
    auto javaClass = jni::java_object<Neuro::StimulationParams>::java_class();

    auto amplitudeMethodId = env->GetMethodID(javaClass, "amplitude", "()I");
    auto amplitude = env->CallIntMethod(obj, amplitudeMethodId);

    auto pulseDurationMethodId = env->GetMethodID(javaClass, "pulseWidth", "()I");
    auto pulseDuration = env->CallIntMethod(obj, pulseDurationMethodId);

    auto frequencyMethodId = env->GetMethodID(javaClass, "frequency", "()I");
    auto frequency = env->CallIntMethod(obj, frequencyMethodId);

    auto stimulusMethodId = env->GetMethodID(javaClass, "stimulusDuration", "()I");
    auto stimulusDuration = env->CallIntMethod(obj, stimulusMethodId);

    return Neuro::StimulationParams{amplitude, pulseDuration, frequency, stimulusDuration};
}

template<>
template<>
jni::java_object<Neuro::StimulatorDeviceState>::java_object(const Neuro::StimulatorDeviceState &device_state):
        nativeObj(device_state){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::StimulatorDeviceState>());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class,
                               objectClassConstructor,
                               static_cast<jobject>(jni::java_object<Neuro::StimulatorDeviceState::State>(device_state.StimulatorState)),
                               static_cast<jobject>(jni::java_object<Neuro::StimulatorDeviceState::State>(device_state.MAState))));
    });
}

template<>
template<>
jni::java_object<Neuro::MotionAssistantParams>::java_object(const Neuro::MotionAssistantParams &params):
        nativeObj(params){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::MotionAssistantParams>());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class,
                               objectClassConstructor,
                               params.gyroStart,
                               params.gyroStop,
                               static_cast<jobject>(jni::java_object<Neuro::MotionAssistantLimb>(params.limb)),
                               params.minPause));
    });
}

template <>
Neuro::MotionAssistantParams jni::get_java_obj_value<Neuro::MotionAssistantParams>(JNIEnv *env, jobject obj){
    auto javaClass = jni::java_object<Neuro::MotionAssistantParams>::java_class();

    auto gyroStartMethodId = env->GetMethodID(javaClass, "gyroStartAngle", "()I");
    auto gyroStart = env->CallIntMethod(obj, gyroStartMethodId);

    auto gyroStopMethodId = env->GetMethodID(javaClass, "gyroStopAngle", "()I");
    auto gyroStop = env->CallIntMethod(obj, gyroStopMethodId);

    auto methodSignature = std::string("()L") + jni::java_class_name<Neuro::MotionAssistantLimb>() + ";";
    auto limbMethodId = env->GetMethodID(javaClass, "limb", methodSignature.c_str());
    auto limbObj = env->CallObjectMethod(obj, limbMethodId);
    auto limb = jni::enumFromJavaObj<Neuro::MotionAssistantLimb>(env, limbObj);

    auto minPauseMethodId = env->GetMethodID(javaClass, "minPause", "()I");
    auto minPause = env->CallIntMethod(obj, minPauseMethodId);

    return Neuro::MotionAssistantParams{gyroStart, gyroStop, limb, minPause};
}

template<>
template<>
jni::java_object<Neuro::FirmwareVersion>::java_object(const Neuro::FirmwareVersion &firmware):
        nativeObj(firmware){
    jni::call_in_attached_thread([=](JNIEnv* env){
        auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                       constructor_signature<Neuro::FirmwareVersion>());
        javaObj = make_global_ref_ptr(
                env->NewObject(object_class,
                               objectClassConstructor,
                               firmware.Version,
                               firmware.Build
                ));
    });
}

template <>
Neuro::FirmwareVersion jni::get_java_obj_value<Neuro::FirmwareVersion>(JNIEnv *env, jobject obj){
    auto javaClass = jni::java_object<Neuro::FirmwareVersion>::java_class();

    auto versionMethodId = env->GetMethodID(javaClass, "version", "()I");
    auto version = env->CallIntMethod(obj, versionMethodId);

    auto buildMethodId = env->GetMethodID(javaClass, "build", "()I");
    auto build = env->CallIntMethod(obj, buildMethodId);

    return Neuro::FirmwareVersion{saturation_cast<unsigned>(version), saturation_cast<unsigned>(build)};
}

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
const std::map<Neuro::AccelerometerSensitivity, std::string>
        jni::enum_name_map<Neuro::AccelerometerSensitivity>::mEnumToNameMap = []() {
    return std::map<Neuro::AccelerometerSensitivity, std::string>{
            {Neuro::AccelerometerSensitivity::Sens2g,  "Sens2g"},
            {Neuro::AccelerometerSensitivity::Sens4g,  "Sens4g"},
            {Neuro::AccelerometerSensitivity::Sens8g,  "Sens8g"},
            {Neuro::AccelerometerSensitivity::Sens16g, "Sens16g"}
    };
}();

template<>
const std::map<std::string, Neuro::AccelerometerSensitivity>
        jni::enum_name_map<Neuro::AccelerometerSensitivity>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::AccelerometerSensitivity>{
            {"Sens2g",  Neuro::AccelerometerSensitivity::Sens2g},
            {"Sens4g",  Neuro::AccelerometerSensitivity::Sens4g},
            {"Sens8g",  Neuro::AccelerometerSensitivity::Sens8g},
            {"Sens16g", Neuro::AccelerometerSensitivity::Sens16g}
    };
}();

template<>
const std::map<Neuro::GyroscopeSensitivity, std::string>
        jni::enum_name_map<Neuro::GyroscopeSensitivity>::mEnumToNameMap = []() {
    return std::map<Neuro::GyroscopeSensitivity, std::string>{
            {Neuro::GyroscopeSensitivity::Sens250Grad,  "Sens250Grad"},
            {Neuro::GyroscopeSensitivity::Sens500Grad,  "Sens500Grad"},
            {Neuro::GyroscopeSensitivity::Sens1000Grad, "Sens1000Grad"},
            {Neuro::GyroscopeSensitivity::Sens2000Grad, "Sens2000Grad"}
    };
}();

template<>
const std::map<std::string, Neuro::GyroscopeSensitivity>
        jni::enum_name_map<Neuro::GyroscopeSensitivity>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::GyroscopeSensitivity>{
            {"Sens250Grad",  Neuro::GyroscopeSensitivity::Sens250Grad},
            {"Sens500Grad",  Neuro::GyroscopeSensitivity::Sens500Grad},
            {"Sens1000Grad", Neuro::GyroscopeSensitivity::Sens1000Grad},
            {"Sens2000Grad", Neuro::GyroscopeSensitivity::Sens2000Grad}
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
const std::map<Neuro::StimulatorDeviceState::State, std::string>
        jni::enum_name_map<Neuro::StimulatorDeviceState::State>::mEnumToNameMap = []() {
    return std::map<Neuro::StimulatorDeviceState::State, std::string>{
            {Neuro::StimulatorDeviceState::State::NoParams , "NoParams"},
            {Neuro::StimulatorDeviceState::State::Disabled,  "Disabled"},
            {Neuro::StimulatorDeviceState::State::Enabled, "Enabled"}
    };
}();

template<>
const std::map<std::string, Neuro::StimulatorDeviceState::State>
        jni::enum_name_map<Neuro::StimulatorDeviceState::State>::mNameToEnumMap = []() {
    return std::map<std::string, Neuro::StimulatorDeviceState::State>{
            {"NoParams", Neuro::StimulatorDeviceState::State::NoParams},
            {"Disabled", Neuro::StimulatorDeviceState::State::Disabled},
            {"Enabled", Neuro::StimulatorDeviceState::State::Enabled}
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
            return "Boolean";
        case Neuro::Parameter::StimulatorAndMAState:
            return "StimulatorDeviceState";
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
        case Neuro::Parameter::AccelerometerSens:
            return "AccelerometerSensitivity";
        case Neuro::Parameter::GyroscopeSens:
            return "GyroscopeSensitivity";
        case Neuro::Parameter::StimulatorParamPack:
            return "StimulationParams";
        case Neuro::Parameter::MotionAssistantParamPack:
            return "MotionAssistantParams";
        case Neuro::Parameter::FirmwareVersion:
            return "FirmwareVersion";
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
