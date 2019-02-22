#include "java_helper.h"

extern "C"
{
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetMotionAssistantParamPack(JNIEnv *env, jclass, jlong devicePtr, jobject maParams) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try {
        const auto &maParamsClass = global_class_refs().fromClassName(
                "com/neuromd/neurosdk/parameters/MotionAssistantParams");
        auto gyroStartMethodId = env->GetMethodID(maParamsClass, "gyroStartAngle", "()I");
        auto gyroStart = env->CallIntMethod(maParams, gyroStartMethodId);

        auto gyroStopMethodId = env->GetMethodID(maParamsClass, "gyroStopAngle", "()I");
        auto gyroStop = env->CallIntMethod(maParams, gyroStopMethodId);

        auto limbMethodId = env->GetMethodID(maParamsClass, "limb",
                                             "()Lcom/neuromd/neurosdk/parameters/MotionAssistantLimb;");
        auto limbObj = env->CallObjectMethod(maParams, limbMethodId);
        auto limb = enum_from_java_obj<MotionAssistantLimb>(env, limbObj);

        auto minPauseMethodId = env->GetMethodID(maParamsClass, "minPause", "()I");
        auto minPause = env->CallIntMethod(maParams, minPauseMethodId);

        MotionAssistantParams nativeMaParams;
        nativeMaParams.gyroStart = gyroStart;
        nativeMaParams.gyroStop = gyroStop;
        nativeMaParams.limb = limb;
        nativeMaParams.minPause = minPause;

        throw_if_error(env, device_set_MotionAssistantParamPack(device, nativeMaParams));
    }
    catch (std::exception &e){
        java_throw(env, e.what());
    }
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetStimulatorParamPack(JNIEnv *env, jclass, jlong devicePtr, jobject stimulParams) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try {
        const auto &stimulationParamsClass = global_class_refs().fromClassName(
                "com/neuromd/neurosdk/parameters/MotionAssistantParams");

        auto amplitudeMethodId = env->GetMethodID(stimulationParamsClass, "amplitude", "()I");
        auto amplitude = env->CallIntMethod(stimulParams, amplitudeMethodId);

        auto pulseDurationMethodId = env->GetMethodID(stimulationParamsClass, "pulseWidth", "()I");
        auto pulseWidth = env->CallIntMethod(stimulParams, pulseDurationMethodId);

        auto frequencyMethodId = env->GetMethodID(stimulationParamsClass, "frequency", "()I");
        auto frequency = env->CallIntMethod(stimulParams, frequencyMethodId);

        auto stimulusMethodId = env->GetMethodID(stimulationParamsClass, "stimulusDuration", "()I");
        auto stimulusDuration = env->CallIntMethod(stimulParams, stimulusMethodId);

        StimulationParams nativeStimulationParams;
        nativeStimulationParams.stimulus_duration = stimulusDuration;
        nativeStimulationParams.frequency = frequency;
        nativeStimulationParams.current = amplitude;
        nativeStimulationParams.pulse_width = pulseWidth;

        throw_if_error(env, device_set_StimulatorParamPack(device, nativeStimulationParams));
    }
    catch (std::exception &e){
        java_throw(env, e.what());
    }
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetGyroscopeSens(JNIEnv *env, jclass, jlong devicePtr, jobject gyroSens) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto sensitivity = enum_from_java_obj<GyroscopeSensitivity>(env, gyroSens);
    throw_if_error(env, device_set_GyroscopeSens(device, sensitivity));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetAccelerometerSens(JNIEnv *env, jclass, jlong devicePtr, jobject accelSens) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto sensitivity = enum_from_java_obj<AccelerometerSensitivity>(env, accelSens);
    throw_if_error(env, device_set_AccelerometerSens(device, sensitivity));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetADCInputState(JNIEnv *env, jclass, jlong devicePtr, jobject adcInput) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto adcInputNative = enum_from_java_obj<ADCInput>(env, adcInput);
    throw_if_error(env, device_set_ADCInputState(device, adcInputNative));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetExternalSwitchState(JNIEnv *env, jclass, jlong devicePtr, jobject extSwitch) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto externalSwitch = enum_from_java_obj<ExternalSwitchInput>(env, extSwitch);
    throw_if_error(env, device_set_ExternalSwitchState(device, externalSwitch));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetOffset(JNIEnv *env, jclass, jlong devicePtr, jbyte offset) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    if (offset < 0){
        java_throw(env, "Invalid offset value");
        return;
    }

    throw_if_error(env, device_set_Offset(device, static_cast<unsigned char>(offset)));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetGain(JNIEnv *env, jclass, jlong devicePtr, jobject gain) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto nativeGain = enum_from_java_obj<Gain>(env, gain);
    throw_if_error(env, device_set_Gain(device, nativeGain));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetSamplingFrequency(JNIEnv *env, jclass, jlong devicePtr, jobject freq) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto nativeFreq = enum_from_java_obj<SamplingFrequency>(env, freq);
    throw_if_error(env, device_set_SamplingFrequency(device, nativeFreq));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetFirmwareMode(JNIEnv *env, jclass, jlong devicePtr, jobject mode) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    auto firmwareMode = enum_from_java_obj<FirmwareMode>(env, mode);
    throw_if_error(env, device_set_FirmwareMode(device, firmwareMode));
}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetHardwareFilterState(JNIEnv *env, jclass, jlong devicePtr, jboolean isEnabled) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    throw_if_error(env, device_set_HardwareFilterState(device, isEnabled));
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadMotionAssistantParamPack(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    MotionAssistantParams maParams;
    auto res = device_read_MotionAssistantParamPack(device, &maParams);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadStimulatorParamPack(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadStimulatorAndMAState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadGyroscopeSens(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadAccelerometerSens(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadADCInputState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadExternalSwitchState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jbyte JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadOffset(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadGain(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadSamplingFrequency(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadFirmwareMode(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jboolean JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadHardwareFilterState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadSerialNumber(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);


    return env->NewStringUTF(returnValue);
}

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadAddress(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);


    return env->NewStringUTF(returnValue);
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadName(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);
    return env->NewStringUTF(returnValue);
}
}

