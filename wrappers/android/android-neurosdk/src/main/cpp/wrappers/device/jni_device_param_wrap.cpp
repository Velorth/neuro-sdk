#include "java_helper.h"

extern "C"
{
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetMotionAssistantParamPack(JNIEnv *env, jclass, jlong devicePtr, jobject maParams) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    try {
        const auto &maParamsClass = global_class_refs().fromClassName(
                "com/neuromd/neurosdk/parameters/types/MotionAssistantParams");
        auto gyroStartMethodId = env->GetMethodID(maParamsClass, "gyroStartAngle", "()I");
        auto gyroStart = env->CallIntMethod(maParams, gyroStartMethodId);

        auto gyroStopMethodId = env->GetMethodID(maParamsClass, "gyroStopAngle", "()I");
        auto gyroStop = env->CallIntMethod(maParams, gyroStopMethodId);

        auto limbMethodId = env->GetMethodID(maParamsClass, "limb",
                                             "()Lcom/neuromd/neurosdk/parameters/types/MotionAssistantLimb;");
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
                "com/neuromd/neurosdk/parameters/types/MotionAssistantParams");

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

    try {
        const auto &maParamsClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/MotionAssistantParams");
        const auto &limbClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/MotionAssistantLimb");
        auto limbObject = get_enum_field_ref(env, limbClass, get_enum_name(maParams.limb).c_str());
        auto constructor = env->GetMethodID(maParamsClass, "<init>", "(IILcom/neuromd/neurosdk/parameters/types/MotionAssistantLimb;I)V");
        return env->NewObject(maParamsClass, constructor, maParams.gyroStart, maParams.gyroStop, limbObject, maParams.minPause);
    }
    catch(std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadStimulatorParamPack(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    StimulationParams stimulationParams;
    auto res = device_read_StimulatorParamPack(device, &stimulationParams);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &stimulationParamsClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/StimulationParams");
        auto constructor = env->GetMethodID(stimulationParamsClass, "<init>", "(IIII)V");
        return env->NewObject(stimulationParamsClass, constructor, stimulationParams.current, stimulationParams.pulse_width, stimulationParams.frequency, stimulationParams.stimulus_duration);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadStimulatorAndMAState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    StimulatorAndMaState stimulatorMaState;
    auto res = device_read_StimulatorAndMAState(device, &stimulatorMaState);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &deviceStateClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/StimulatorDeviceState");
        auto constructor = env->GetMethodID(deviceStateClass, "<init>", "(Lcom/neuromd/neurosdk/parameters/types/StimulatorDeviceState$State;Lcom/neuromd/neurosdk/parameters/types/StimulatorDeviceState$State;)V");
        const auto &stateClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/StimulatorDeviceState$State");
        auto stimulatorState = get_enum_field_ref(env, stateClass, get_enum_name(stimulatorMaState.StimulatorState).c_str());
        auto maState = get_enum_field_ref(env, stateClass, get_enum_name(stimulatorMaState.MAState).c_str());
        return env->NewObject(deviceStateClass, constructor, stimulatorState, maState);
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadGyroscopeSens(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    GyroscopeSensitivity gyroSens;
    auto res = device_read_GyroscopeSens(device, &gyroSens);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &gyroSensClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/GyroscopeSensitivity");
        return get_enum_field_ref(env, gyroSensClass, get_enum_name(gyroSens).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadAccelerometerSens(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    AccelerometerSensitivity accelSens;
    auto res = device_read_AccelerometerSens(device, &accelSens);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &accelSensClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/AccelerometerSensitivity");
        return get_enum_field_ref(env, accelSensClass, get_enum_name(accelSens).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadADCInputState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    ADCInput adcInput;
    auto res = device_read_ADCInputState(device, &adcInput);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &adcInputClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/ADCInput");
        return get_enum_field_ref(env, adcInputClass, get_enum_name(adcInput).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadExternalSwitchState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    ExternalSwitchInput externalInput;
    auto res = device_read_ExternalSwitchState(device, &externalInput);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &extInputClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/ExternalSwitchInput");
        return get_enum_field_ref(env, extInputClass, get_enum_name(externalInput).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jbyte JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadOffset(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    unsigned char offset;
    auto res = device_read_Offset(device, &offset);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    return offset;
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadGain(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    Gain gain;
    auto res = device_read_Gain(device, &gain);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &gainClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/Gain");
        return get_enum_field_ref(env, gainClass, get_enum_name(gain).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadSamplingFrequency(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    SamplingFrequency samplingFrequency;
    auto res = device_read_SamplingFrequency(device, &samplingFrequency);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &frequencyClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/SamplingFrequency");
        return get_enum_field_ref(env, frequencyClass, get_enum_name(samplingFrequency).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadFirmwareMode(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    FirmwareMode firmwareMode;
    auto res = device_read_FirmwareMode(device, &firmwareMode);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &modeClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/FirmwareMode");
        return get_enum_field_ref(env, modeClass, get_enum_name(firmwareMode).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jboolean JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadHardwareFilterState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    bool hardwareFilterState;
    auto res = device_read_HardwareFilterState(device, &hardwareFilterState);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }
    return static_cast<jboolean>(hardwareFilterState);
}

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadSerialNumber(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    char serialNumber[256];
    auto res = device_read_SerialNumber(device, serialNumber, 256);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    return env->NewStringUTF(serialNumber);
}

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadAddress(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    char address[256];
    auto res = device_read_Address(device, address, 256);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    return env->NewStringUTF(address);
}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadState(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    DeviceState deviceState;
    auto res = device_read_State(device, &deviceState);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    try {
        const auto &stateClass = global_class_refs().fromClassName("com/neuromd/neurosdk/parameters/types/DeviceState");
        return get_enum_field_ref(env, stateClass, get_enum_name(deviceState).c_str());
    }
    catch (std::exception &e){
        java_throw(env, e.what());
        return nullptr;
    }
}

JNIEXPORT jstring JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadName(JNIEnv *env, jclass, jlong devicePtr) {
    auto device = reinterpret_cast<Device *>(devicePtr);

    char name[256];
    auto res = device_read_Name(device, name, 256);
    if (res != SDK_NO_ERROR){
        throw_if_error(env, res);
        return nullptr;
    }

    return env->NewStringUTF(name);
}
}

