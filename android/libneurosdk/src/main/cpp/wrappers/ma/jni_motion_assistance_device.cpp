#include <wrappers/jni_ptr_wrap.h>
#include <wrappers/jni_neuro_device_wrap.h>
#include <wrappers/jni_motion_assistant_device_wrap.h>
#include "java_environment.h"
#include "method/ma_device.h"

extern "C"
{


JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_deleteMotionAssistantObj(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto motionAssistantDevice = reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    (*motionAssistantDevice)->getNeuroDevice()->disconnect();
    delete motionAssistantDevice;
}

/**
 * Motion assistant start
 */
JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_motionAssistantStart__J(
        JNIEnv *env, jobject instance, jlong objPtr) {

    // TODO make wrappers inside of core for all functions

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->motionAssistantStart();
}

/**
 * Motion assistant stop
 */
JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_motionAssistantStop__J(JNIEnv *env,
                                                                                        jobject instance,
                                                                                        jlong objPtr) {


    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->motionAssistantStop();
}

JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getName(JNIEnv *env,
                                                                            jobject instance,
                                                                            jlong objPtr) {


    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    auto name = motionAssistantDevice->getNeuroDevice()->getName();

    return env->NewStringUTF(name.c_str());
}

JNIEXPORT jstring JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getDeviceAddress(JNIEnv *env,
                                                                                  jobject instance,
                                                                                  jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    auto address = motionAssistantDevice->getNeuroDevice()->getAddress();

    return env->NewStringUTF(address.c_str());
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getBatteryLevel(JNIEnv *env,
                                                                                    jobject instance,
                                                                                    jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getNeuroDevice()->getBatteryLevel();
}

JNIEXPORT jobject JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getNeuroDevice(JNIEnv *env,
                                                                                   jobject instance,
                                                                                   jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    auto neuroDevice = new JniNeuroDevice(motionAssistantDevice->getNeuroDevice());

    jni::java_object<decltype(neuroDevice)> deviceWrapObj(neuroDevice);

    neuroDevice->subscribeStateChanged(find_notifier<decltype(neuroDevice)>(deviceWrapObj, "deviceStateChanged"));
    neuroDevice->subscribeBatteryLevelChanged(find_notifier<decltype(neuroDevice)>(deviceWrapObj, "batteryLevelChanged"));

    return env->NewLocalRef(deviceWrapObj);
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getPulseFrequency__J(JNIEnv *env,
                                                                                      jobject instance,
                                                                                      jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getPulseFrequency();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getPulseDuration__J(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getPulseDuration();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getCurrentAmplitude__J(JNIEnv *env,
                                                                                        jobject instance,
                                                                                        jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getCurrentAmplitude();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getStimulDuration(JNIEnv *env,
                                                                                   jobject instance,
                                                                                   jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getStimulationDuration();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setPulseFrequency__JI(JNIEnv *env,
                                                                                       jobject instance,
                                                                                       jlong objPtr,
                                                                                       jint frequency) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setPulseFrequency(frequency);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setPulseDuration__JI(JNIEnv *env,
                                                                                      jobject instance,
                                                                                      jlong objPtr,
                                                                                      jint duration) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setPulseDuration(duration);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setCurrentAmplitude__JI(
        JNIEnv *env, jobject instance, jlong objPtr, jint amplitude) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setCurrentAmplitude(amplitude);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setStimulatorParams(
        JNIEnv *env, jobject instance, jlong objPtr, jint amplitude, jint pulse_duration,
        jint frequency, jint stimul_duration) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setStimulatorParams(amplitude, pulse_duration, frequency, stimul_duration);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setStimulationDuration__JI(
        JNIEnv *env, jobject instance, jlong objPtr, jint stimul_duration) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setStimulationDuration(stimul_duration);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_stimulationStop__J(JNIEnv *env,
                                                                                    jobject instance,
                                                                                    jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->stimulationStop();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_stimulationStart__J(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->stimulationStart();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getAnalyzedAxis(JNIEnv *env,
                                                                                 jobject instance,
                                                                                 jlong objPtr) {

    //auto motionAssistantDevice = *(std::shared_ptr<MotionAssistantDevice> *) objPtr;
    //return motionAssistantDevice->getAnalyzedAxis();
    return 0;
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getGyroStopThreshold__J(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getGyroStopThreshold();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getGyroStartThreshold__J(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getGyroStartThreshold();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getLimbForStimulation(JNIEnv *env,
                                                                                       jobject instance,
                                                                                       jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    auto limb = motionAssistantDevice->getLimbForStimulation();
    return static_cast<jint>(limb);
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getMinAssistantStimulationPause(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getMinAssistantStimulationPause();
}

JNIEXPORT jint JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getMaxAssistantStimulusDuration(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    return motionAssistantDevice->getMaxAssistantStimulusDuration();
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setGyroStopThreshold__JI(
        JNIEnv *env, jobject instance, jlong objPtr, jint gyroStop) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setGyroStopThreshold(gyroStop);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setGyroStartThreshold__JI(
        JNIEnv *env, jobject instance, jlong objPtr, jint gyroStart) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setGyroStartThreshold(gyroStart);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setLimbForStimulation(JNIEnv *env,
                                                                                       jobject instance,
                                                                                       jlong objPtr,
                                                                                       jint limbCode) {
    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    MotionAssistantLimb limb;
    parseLimbCode(limbCode, limb);
    motionAssistantDevice->setLimbForStimulation(limb);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setMaxAssistantStimulusDuration(
        JNIEnv *env, jobject instance, jlong objPtr, jint maxDuration) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setMaxAssistantStimulusDuration(maxDuration);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setMinAssistantStimulationPause(
        JNIEnv *env, jobject instance, jlong objPtr, jint minPause) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->setMinAssistantStimulationPause(minPause);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_setMotionAssistantParams(
        JNIEnv *env, jobject instance, jlong objPtr, jint gyroStart, jint gyroStop, jint limbCode,
        jint minPause, jint maxDuration) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    MotionAssistantLimb limb;
    parseLimbCode(limbCode, limb);
    motionAssistantDevice->setMotionAssistantParams(gyroStart, gyroStop, limb, minPause, maxDuration);
}


JNIEXPORT jboolean JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getMotionAssistantState(
        JNIEnv *env, jobject instance, jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    auto assistantIsActive = motionAssistantDevice->motionAssistantIsActive();
    return static_cast<jboolean>(assistantIsActive);
}

JNIEXPORT jboolean JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_getStimulatorState__J(JNIEnv *env,
                                                                                       jobject instance,
                                                                                       jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    auto stimulatorIsActive = motionAssistantDevice->stimulatorIsActive();
    return static_cast<jboolean>(stimulatorIsActive);
}

JNIEXPORT void JNICALL
Java_ru_neurotech_neurodevices_stimulation_MotionAssistantDevice_doCalibration(JNIEnv *env,
                                                                               jobject instance,
                                                                               jlong objPtr) {

    auto& motionAssistantDevice = *reinterpret_cast<JniMotionAssistantDevice *>(objPtr);
    motionAssistantDevice->doCalibration();
}

}