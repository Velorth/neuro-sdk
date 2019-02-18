#include <jni.h>
#include "jni_logger.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
#ifdef NDEBUG
    logFactory->setLogLevel(LogLevel::Info);
#else
    logFactory->setLogLevel(LogLevel::Trace);
#endif
    return JNI_VERSION_1_6;
}