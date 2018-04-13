/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
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
#include "java_environment.h"

JavaVM *jni::java_machine;

int jni::get_env(JNIEnv **g_env) {
    int getEnvStat = java_machine->GetEnv((void **) g_env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        __android_log_print(ANDROID_LOG_VERBOSE, "GetEnvironmentRoutine",
                            "Thread detached. Attaching to JVM");
        if (java_machine->AttachCurrentThread(g_env, nullptr) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "GetEnvironmentRoutine", "FAILED ATTACH THREAD");
            return 2; //Failed attach
        }
        return 1; //Attached. Need detach
    }
    __android_log_print(ANDROID_LOG_VERBOSE, "GetEnvironmentRoutine",
                        "Thread already attached to JVM");
    return 0;//Already attached
}

void jni::detach_thread() {
    __android_log_print(ANDROID_LOG_VERBOSE, "GetEnvironmentRoutine",
                        "Detaching thread");
    java_machine->DetachCurrentThread();
}

void jni::delete_global_ref(jobject ref){

    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return;

    if (env->GetObjectRefType(ref) != JNIGlobalRefType)
        return;

    env->DeleteGlobalRef(ref);

    if (resCode == 1) jni::detach_thread();
}

std::shared_ptr<jni::jobject_t> jni::make_global_ref_ptr(jobject localRef){
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) std::shared_ptr<jni::jobject_t>();

    auto globalRef = env->NewGlobalRef(localRef);

    if (resCode == 1) jni::detach_thread();
    return std::shared_ptr<jni::jobject_t>(globalRef, delete_global_ref);
}