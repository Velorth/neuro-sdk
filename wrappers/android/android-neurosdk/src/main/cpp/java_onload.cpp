#include "java_helper.h"
#include "jni_logger.h"

struct GlobalEnvHolder{
    static JNIEnv* get(){
        if (Env == nullptr)
            throw std::runtime_error("Attempt to access java environment before JavaOnLoad");
        return Env;
    }
private:
    static JNIEnv *Env;

    friend jint JNI_OnLoad(JavaVM *, void *);
    static void set(JNIEnv *env){
        Env = env;
    }
};

const SdkJavaClasses& global_class_refs(){
    static SdkJavaClasses javaClasses{GlobalEnvHolder::get()};
    return javaClasses;
}

class JniLogger : public Logger {
    static constexpr const char *NeuroCoreLogTag = "NeuroCore";
public:
    void trace(const char *format, ...){
        if (currentLogLevel > LogLevel::Trace) return;

        va_list formatData;
        va_start(formatData, format);
        __android_log_vprint(ANDROID_LOG_VERBOSE, NeuroCoreLogTag, format, formatData);
        va_end(formatData);
    }

    void debug(const char *format, ...){
        if (currentLogLevel > LogLevel::Info) return;

        va_list formatData;
        va_start(formatData, format);
        __android_log_vprint(ANDROID_LOG_INFO, NeuroCoreLogTag, format, formatData);
        va_end(formatData);
    }

    void info(const char *format, ...){
        if (currentLogLevel > LogLevel::Debug) return;

        va_list formatData;
        va_start(formatData, format);
        __android_log_vprint(ANDROID_LOG_DEBUG, NeuroCoreLogTag, format, formatData);
        va_end(formatData);
    }

    void warn(const char *format, ...){
        if (currentLogLevel > LogLevel::Warn) return;

        va_list formatData;
        va_start(formatData, format);
        __android_log_vprint(ANDROID_LOG_WARN, NeuroCoreLogTag, format, formatData);
        va_end(formatData);
    }

    void error(const char *format, ...){
        if (currentLogLevel > LogLevel::Error) return;

        va_list formatData;
        va_start(formatData, format);
        __android_log_vprint(ANDROID_LOG_ERROR, NeuroCoreLogTag, format, formatData);
        va_end(formatData);
    }
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    GlobalEnvHolder::set(env);

    auto logFactory = LoggerFactory::getInstance();
    logFactory->setLogger(new JniLogger());
#ifdef NDEBUG
    logFactory->setLogLevel(LogLevel::Info);
#else
    logFactory->setLogLevel(LogLevel::Trace);
#endif
    return JNI_VERSION_1_6;
}