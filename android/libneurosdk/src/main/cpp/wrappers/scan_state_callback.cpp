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

#include "logger.h"
#include "wrappers/scan_state_callback.h"
#include "java_environment.h"

void javaOnScanStateChanged(void *subscriber, bool isScanning) {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return;

    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Notifying scan state changed: %s", "ScanStateCallback", __FUNCTION__, isScanning?"TRUE":"FALSE");
    auto subscriberClass = env->GetObjectClass((jobject) subscriber);
    auto callbackMethod = env->GetMethodID(subscriberClass, "onScanStateChanged",
                                           "(Z)V");

    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Calling java callback", "ScanStateCallback", __FUNCTION__);
    env->CallVoidMethod((jobject) subscriber, callbackMethod, (jboolean)isScanning);

    if (resCode == 1) jni::detach_thread();
}