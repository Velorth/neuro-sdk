/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neuromd.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android/log.h>
#include <java_environment.h>

JavaVM *jni::java_machine;

int jni::get_env(JNIEnv **g_env) {
    int getEnvStat = java_machine->GetEnv((void **) g_env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        if (java_machine->AttachCurrentThread(g_env, nullptr) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "GetEnvironmentRoutine", "FAILED ATTACH THREAD");
            return 2; //Failed attach
        }
        return 1; //Attached. Need detach
    }
    return 0;//Already attached
}

void jni::detach_thread() {
    java_machine->DetachCurrentThread();
}

void jni::delete_global_ref(jobject ref) noexcept {
    try {
        jni::call_in_attached_thread([=](auto env) {
            if (env->GetObjectRefType(ref) != JNIGlobalRefType)
                return;

            env->DeleteGlobalRef(ref);
        });
    }
    catch (std::exception &e){
        __android_log_print(ANDROID_LOG_FATAL, "DeleteGlobalRef",
                            "Error deleting global ref: %s", e.what());
    }
}

std::shared_ptr<jni::jobject_t> jni::make_global_ref_ptr(jobject localRef){
    return jni::call_in_attached_thread([=](auto env) {
        auto globalRef = env->NewGlobalRef(localRef);
        return std::shared_ptr<jni::jobject_t>(globalRef, delete_global_ref);
    });
}

template<>
std::string jni::get_java_obj_value<std::string>(JNIEnv *env, jobject obj){
    if(env->IsInstanceOf(obj, jni::java_object<std::string>::java_class()) == JNI_FALSE) {
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("Parameter is not a string"));
        return nullptr;
    }
    auto str = static_cast<jstring>(obj);
    auto chars = env->GetStringUTFChars(str, 0);
    std::string result(chars);
    env->ReleaseStringUTFChars(str, chars);
    return result;
}

template<>
int jni::get_java_obj_value<int>(JNIEnv *env, jobject obj){
    if(env->IsInstanceOf(obj, jni::java_object<int>::java_class()) == JNI_FALSE) {
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("Parameter is not of int type"));
        return 0;
    }
    auto getIntMethodID = env->GetMethodID(jni::java_object<int>::java_class(), "intValue", "()I");
    auto intValue = env->CallIntMethod(obj, getIntMethodID);
    return intValue;
}

template<>
long jni::get_java_obj_value<long>(JNIEnv *env, jobject obj){
    if(env->IsInstanceOf(obj, jni::java_object<long>::java_class()) == JNI_FALSE) {
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("Parameter is not of long type"));
        return 0;
    }
    auto getLongMethodID = env->GetMethodID(jni::java_object<long>::java_class(), "longValue", "()J");
    auto longValue = env->CallLongMethod(obj, getLongMethodID);
    return longValue;
}

template<>
double jni::get_java_obj_value<double>(JNIEnv *env, jobject obj){
    if(env->IsInstanceOf(obj, jni::java_object<double>::java_class()) == JNI_FALSE) {
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("Parameter is not of double type"));
        return 0.0;
    }
    auto getDoubleMethodID = env->GetMethodID(jni::java_object<double>::java_class(),
                                            "doubleValue", "()D");
    auto doubleValue = env->CallDoubleMethod(obj, getDoubleMethodID);
    return doubleValue;
}

template<>
bool jni::get_java_obj_value<bool>(JNIEnv *env, jobject obj){
    if(env->IsInstanceOf(obj, jni::java_object<bool>::java_class()) == JNI_FALSE) {
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("Parameter is not of boolean type"));
        return false;
    }
    auto getBoolMethodID = env->GetMethodID(jni::java_object<bool>::java_class(),
                                            "booleanValue", "()Z");
    auto boolValue = env->CallBooleanMethod(obj, getBoolMethodID);
    return boolValue;
}

template<>
unsigned char jni::get_java_obj_value<unsigned char>(JNIEnv *env, jobject obj){
    if(env->IsInstanceOf(obj, jni::java_object<unsigned char>::java_class()) == JNI_FALSE) {
        jni::java_throw(env,
                        "java/lang/IllegalArgumentException",
                        std::runtime_error("Parameter is not of byte type"));
        return 0;
    }
    auto getByteMethodID = env->GetMethodID(jni::java_object<unsigned char>::java_class(),
                                            "byteValue", "()B");
    auto byteValue = env->CallByteMethod(obj, getByteMethodID);
    return static_cast<unsigned char>(byteValue);
}

template<>
std::size_t jni::get_java_obj_value<std::size_t>(JNIEnv *env, jobject size){
    return static_cast<std::size_t>(get_java_obj_value<long>(env, size));
}

template<>
template<>
jni::java_object<std::string>::java_object(const std::string &str):nativeObj(str){
    call_in_attached_thread([=](JNIEnv *env){
        javaObj = make_global_ref_ptr(env->NewStringUTF(str.c_str()));
    });
}