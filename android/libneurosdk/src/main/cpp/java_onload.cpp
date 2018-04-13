#include "wrappers/jni_eeg_device_wrap.h"
#include "wrappers/jni_ecg_device_wrap.h"
#include "wrappers/jni_bfb_device_wrap.h"
#include "wrappers/jni_motion_assistant_device_wrap.h"
#include "wrappers/jni_neuro_device_wrap.h"
#include "jni_logger.h"

jobject _channelClass;

jclass jni::getChannelClass() {
    return (jclass) _channelClass;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jni::java_machine = vm;
    JNIEnv *env;
    jni::java_machine->GetEnv((void **) &env, JNI_VERSION_1_6);

    jni::initJavaObjClass<int>(env);
    jni::initJavaObjClass<double>(env);
    jni::initJavaObjClass<bool>(env);
    jni::initJavaObjClass<DeviceState>(env);
    jni::initJavaObjClass<RPeak>(env);
    jni::initJavaObjClass<ArtifactZone>(env);
    jni::initJavaObjClass<MotionAssistantLimb>(env);
    jni::initJavaObjClass<MotionAssistantParams>(env);
    jni::initJavaObjClass<StimulationParams>(env);
    //initJavaObjClass<SpectrumData>(env);
    jni::initJavaObjClass<JniEegDevice *>(env);
    jni::initJavaObjClass<JniNeuroDevice *>(env);
    jni::initJavaObjClass<JniBfbDevice *>(env);
    jni::initJavaObjClass<JniBfbIndex *>(env);
    jni::initJavaObjClass<JniEcgDevice *>(env);
    jni::initJavaObjClass<JniMotionAssistantDevice *>(env);

    _channelClass = env->NewGlobalRef(env->FindClass("ru/neurotech/neurodevices/features/Channel"));

    //_spectrumDataClass = env->NewGlobalRef(env->FindClass("ru/neurotech/neurodevices/eeg/SpectrumData"));

    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
    logFactory->setLogLevel(LogLevel::Debug);
    return JNI_VERSION_1_6;
}