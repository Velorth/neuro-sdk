#include "java_helper.h"

extern "C"
{
JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetMotionAssistantParamPack(JNIEnv *env, jclass, jlong devicePtr, jobject maParams) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetStimulatorParamPack(JNIEnv *env, jclass, jlong devicePtr, jobject stimulParams) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetGyroscopeSens(JNIEnv *env, jclass, jlong devicePtr, jobject gyroSens) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetAccelerometerSens(JNIEnv *env, jclass, jlong devicePtr, jobject accelSens) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetADCInputState(JNIEnv *env, jclass, jlong devicePtr, jobject adcInput) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetExternalSwitchState(JNIEnv *env, jclass, jlong devicePtr, jobject extSwitch) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetOffset(JNIEnv *env, jclass, jlong devicePtr, jbyte offset) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetGain(JNIEnv *env, jclass, jlong devicePtr, jobject gain) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetSamplingFrequency(JNIEnv *env, jclass, jlong devicePtr, jobject freq) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetFirmwareMode(JNIEnv *env, jclass, jlong devicePtr, jobject mode) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT void JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceSetHardwareFilterState(JNIEnv *env, jclass, jlong devicePtr, jboolean isEnabled) {

    auto device = reinterpret_cast<Device *>(devicePtr);

}

JNIEXPORT jobject JNICALL
Java_com_neuromd_neurosdk_ParameterTypeInfo_deviceReadMotionAssistantParamPack(JNIEnv *env, jclass, jlong devicePtr) {

    auto device = reinterpret_cast<Device *>(devicePtr);

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

