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

#ifndef ANDROID_JNI_PTR_WRAP_H
#define ANDROID_JNI_PTR_WRAP_H

#include "device/device.h"
#include "device/device_parameters.h"
#include "java_environment.h"

template<>
constexpr const char *jni::java_class_name<Neuro::DeviceState>() { return "ru/neurotech/neurosdk/state/DeviceState"; };

template<>
constexpr const char *jni::constructor_signature<Neuro::DeviceState>() { return "(I)Lru/neurotech/neurosdk/state/DeviceState;"; };

template<>
template<>
jni::java_object<Neuro::DeviceState>::java_object(const Neuro::DeviceState &);

template <typename JniPtrObj>
jobject find_notifier(jobject java_obj, const char *notifier_name) {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return nullptr;

    auto notifierFieldID = env->GetFieldID(jni::java_object<JniPtrObj>::java_class(), notifier_name, "Lru/neurotech/common/SubscribersNotifier;");
    auto deviceStateChangedNotifier = env->GetObjectField(java_obj, notifierFieldID);

    if (resCode == 1) jni::detach_thread();
    return deviceStateChangedNotifier;
};

inline void callJavaSendNotification(jobject subscriberNotifier, jobject param){
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return;

    auto subscriberClass = env->GetObjectClass(subscriberNotifier);
    auto callbackMethod = env->GetMethodID(subscriberClass, "sendNotification",
                                           "(Ljava/lang/Object;Ljava/lang/Object;)V");
    env->CallVoidMethod(subscriberNotifier, callbackMethod, nullptr, param);

    if (resCode == 1) jni::detach_thread();
}

template <typename T>
void sendNotification(std::weak_ptr<jni::jobject_t> subscriberRefPtr, T param){
    auto subscriberRef = subscriberRefPtr.lock();
    if (!subscriberRef)
        return;

    jni::java_object<T> jObj(param);
    callJavaSendNotification(subscriberRef.get(), jObj);
}

void sendNotification(std::weak_ptr<jni::jobject_t> subscriberRefPtr);

template <class Obj>
class JniPtrWrap {
public:
    JniPtrWrap(const JniPtrWrap &) = delete;
    JniPtrWrap &operator=(const JniPtrWrap &) = delete;
    virtual ~JniPtrWrap() = default;

    typedef std::shared_ptr<Obj> object_ptr_t;

    Obj *operator->() const {
        return object.operator->();
    }
protected:
    JniPtrWrap(std::shared_ptr<Obj> objPtr) : object(objPtr){}
    object_ptr_t object;
};

#endif //ANDROID_JNI_PTR_WRAP_H
