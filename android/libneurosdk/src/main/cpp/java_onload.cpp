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
    jni::initJavaObjClass<JniDeviceWrap *>(env);
    jni::initJavaObjClass<JniBatteryChannelWrap *>(env);
    jni::initJavaObjClass<JniSignalChannelWrap *>(env);
    jni::initJavaObjClass<Neuro::ChannelInfo *>(env);
    jni::initJavaObjClass<Neuro::DeviceState>(env);
    jni::initJavaObjClass<Neuro::Command>(env);
    jni::initJavaObjClass<Neuro::Parameter>(env);
    jni::initJavaObjClass<Neuro::ParamAccess>(env);

    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
    logFactory->setLogLevel(LogLevel::Debug);
    return JNI_VERSION_1_6;
}