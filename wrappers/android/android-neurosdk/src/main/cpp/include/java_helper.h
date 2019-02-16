#ifndef JAVA_HELPER_H
#define JAVA_HELPER_H

#include <functional>
#include <jni.h>
#include <android/log.h>
#include "sdk_error.h"
#include "clistener.h"

static inline bool java_throw(JNIEnv *env, const char *exception_message) {
    auto exceptionClass = env->FindClass("java/lang/UnsupportedOperationException");
    if (exceptionClass == nullptr) {
        return false;
    }
    return !env->ThrowNew(exceptionClass, exception_message);
}

static inline bool throw_if_error(JNIEnv *env, int result_code){
    if (result_code == ERROR_EXCEPTION_WITH_MESSAGE){
        char message[256];
        sdk_last_error_msg(message, 256);
        return java_throw(env, message);
    }
    else if (result_code == ERROR_UNHANDLED_EXCEPTION){
        return java_throw(env, "Unhandled exception");
    }
    return true;
}

class AttachedTaskQueue final{
public:
    AttachedTaskQueue(JNIEnv *);
    AttachedTaskQueue(const AttachedTaskQueue &) = delete;
    AttachedTaskQueue& operator=(const AttachedTaskQueue &) = delete;
    AttachedTaskQueue(AttachedTaskQueue &&) noexcept;
    AttachedTaskQueue& operator=(AttachedTaskQueue &&) noexcept;
    ~TaskQueue();

    void exec(const std::function<void(JNIEnv *)> &);

private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};

class ListenerHelper final {
public:
    ListenerHelper(JNIEnv *env, jobject subscriber, jmethodID callback_method_id):
        mJavaSubscriber(subscriber),
        mCallbackMethodId(callback_method_id),
        mCListener(nullptr),
        mNotificationQueue(env){}
    ListenerHelper(const ListenerHelper &) = delete;
    ListenerHelper& operator=(const ListenerHelper &) = delete;
    ~ListenerHelper(){
        free_listener_handle(mCListener);
    }

    ListenerHandle *listnerHandlePointer(){
        return &mCListener;
    }

    template <typename... Args>
    void notify(Args... args){
        mNotificationQueue.exec([=](JNIEnv *env){
            env->CallVoidMethod(mJavaSubscriber, mCallbackMethodId, args...);
        });
    }

private:
    jobject mJavaSubscriber;
    jmethodID mCallbackMethodId;
    ListenerHandle mCListener;
    AttachedTaskQueue mNotificationQueue;
};

static inline ListenerHelper* make_listener_helper(JNIEnv *env, jobject subscriber, const char *callback_name, const char *callback_signature){
    auto subscriberClass = env->GetObjectClass(subscriber);
    auto callbackMethodId = env->GetMethodID(subscriberClass, callback_name, callback_signature);
    return new ListenerHelper(env, subscriber, callbackMethodId);
}

#endif