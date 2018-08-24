#include "java_environment.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jni::on_vm_load(vm);
    JNIEnv *env;
    if (jni::get_env(&env)!=0)
        std::terminate();

    jni::initJavaObjClass<int>(env);
    jni::initJavaObjClass<long>(env);
    jni::initJavaObjClass<double>(env);
    jni::initJavaObjClass<bool>(env);
    jni::initJavaObjClass<unsigned char>(env);
    jni::initJavaObjClass<std::string>(env);

    return JNI_VERSION_1_6;
}