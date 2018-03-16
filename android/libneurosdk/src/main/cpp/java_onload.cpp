#include "wrappers/channels/jni_battery_channel_wrap.h"
#include "wrappers/channels/jni_base_channel_wrap.h"
#include "wrappers/jni_device_wrap.h"
#include "jni_logger.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jni::java_machine = vm;
    JNIEnv *env;
    jni::java_machine->GetEnv((void **) &env, JNI_VERSION_1_6);

    jni::initJavaObjClass<int>(env);
    jni::initJavaObjClass<double>(env);
    jni::initJavaObjClass<bool>(env);
    jni::initJavaObjClass<JniDeviceWrap *>(env);
    jni::initJavaObjClass<JniBaseChannelWrap *>(env);
    jni::initJavaObjClass<JniBatteryChannelWrap *>(env);

    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
    logFactory->setLogLevel(LogLevel::Debug);
    return JNI_VERSION_1_6;
}