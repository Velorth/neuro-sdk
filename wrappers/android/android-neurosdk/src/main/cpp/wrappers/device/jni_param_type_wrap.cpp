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
