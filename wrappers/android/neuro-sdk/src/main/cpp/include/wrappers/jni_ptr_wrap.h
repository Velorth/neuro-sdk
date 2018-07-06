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

#ifndef ANDROID_JNI_PTR_WRAP_H
#define ANDROID_JNI_PTR_WRAP_H

#include "java_environment.h"

template <typename JniPtrObj>
jobject find_notifier(jobject java_obj, const char *notifier_name) {
    return jni::call_in_attached_thread([=](auto env) {
        auto notifierFieldID = env->GetFieldID(jni::java_object<JniPtrObj>::java_class(),
                                               notifier_name,
                                               "Lcom/neuromd/common/SubscribersNotifier;");
        auto deviceStateChangedNotifier = env->GetObjectField(java_obj, notifierFieldID);
        return deviceStateChangedNotifier;
    });
}

void callJavaSendNotification(JNIEnv *, jobject subscriberNotifier, jobject param);

template <typename T>
void sendNotification(JNIEnv *env, std::weak_ptr<jni::jobject_t> subscriberRefPtr, T param){
    auto subscriberRef = subscriberRefPtr.lock();
    if (!subscriberRef)
        return;

    auto jObj = static_cast<jobject>(jni::java_object<T>(param));
    callJavaSendNotification(env, subscriberRef.get(), jObj);
    env->DeleteLocalRef(jObj);
}

void sendNotification(JNIEnv *, std::weak_ptr<jni::jobject_t> subscriberRefPtr);

template <class Obj>
class JniPtrWrap {
public:
    JniPtrWrap(const JniPtrWrap &) = delete;
    JniPtrWrap &operator=(const JniPtrWrap &) = delete;
    virtual ~JniPtrWrap() = default;

    using object_ptr_t = std::shared_ptr<Obj>;
    using obj_t = Obj;

    Obj* operator->() const {
        return object.operator->();
    }

    object_ptr_t operator*() const {
        return object;
    }

protected:
    JniPtrWrap(object_ptr_t objPtr) : object(objPtr){}
    object_ptr_t object;
};

template <typename T>
T*
extract_pointer(JNIEnv *env, jobject object, const char *pointer_field_name = "mNativeObjPtr"){
    auto objectClass = env->GetObjectClass(object);
    auto pointerFieldID = env->GetFieldID(objectClass, pointer_field_name, "J");
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        return nullptr;
    }
    auto longPtr = env->GetLongField(object, pointerFieldID);
    return reinterpret_cast<T*>(longPtr);
}

template <typename T>
void
deleteNativeObject(JNIEnv *env, jobject object, const char *pointer_field_name = "mNativeObjPtr"){
    delete extract_pointer<T>(env, object, pointer_field_name);
}

#endif //ANDROID_JNI_PTR_WRAP_H
