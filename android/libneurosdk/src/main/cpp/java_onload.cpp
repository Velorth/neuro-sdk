#include "wrappers/device/jni_param_types_wrap.h"
#include "wrappers/channels/jni_battery_channel_wrap.h"
#include "wrappers/channels/jni_signal_channel_wrap.h"
#include "wrappers/channels/jni_channel_info_wrap.h"
#include "wrappers/device/jni_device_wrap.h"
#include "wrappers/device/jni_device_param_wrap.h"
#include "jni_logger.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jni::java_machine = vm;
    JNIEnv *env;
    jni::java_machine->GetEnv((void **) &env, JNI_VERSION_1_6);

    jni::initJavaObjClass<int>(env);
    jni::initJavaObjClass<long>(env);
    jni::initJavaObjClass<double>(env);
    jni::initJavaObjClass<bool>(env);
    jni::initJavaObjClass<unsigned char>(env);
    jni::initJavaObjClass<std::string>(env);
    jni::initJavaObjClass<JniDeviceWrap *>(env);
    jni::initJavaObjClass<JniBatteryChannelWrap *>(env);
    jni::initJavaObjClass<JniSignalChannelWrap *>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo *>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo::Type>(env);
    jni::initJavaObjClass<Neuro::DeviceState>(env);
    jni::initJavaObjClass<Neuro::Command>(env);
    jni::initJavaObjClass<Neuro::Parameter>(env);
    jni::initJavaObjClass<Neuro::ParamAccess>(env);
    jni::initJavaObjClass<Neuro::ParamPair>(env);
    jni::initJavaObjClass<ParameterType>(env);
    jni::initJavaObjClass<Neuro::ADCInput>(env);
    jni::initJavaObjClass<Neuro::ExternalSwitchInput >(env);
    jni::initJavaObjClass<Neuro::FirmwareMode>(env);
    jni::initJavaObjClass<Neuro::Gain>(env);
    jni::initJavaObjClass<Neuro::MotionAssistantLimb>(env);
    jni::initJavaObjClass<Neuro::SamplingFrequency>(env);

    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
    logFactory->setLogLevel(LogLevel::Debug);
    return JNI_VERSION_1_6;
}