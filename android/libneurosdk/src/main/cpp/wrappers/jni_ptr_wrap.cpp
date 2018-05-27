#include "wrappers/jni_ptr_wrap.h"

void callJavaSendNotification(JNIEnv *env, jobject subscriberNotifier, jobject param){
    auto subscriberClass = env->GetObjectClass(subscriberNotifier);
    auto callbackMethod = env->GetMethodID(subscriberClass, "sendNotification",
                                           "(Ljava/lang/Object;Ljava/lang/Object;)V");
    env->CallVoidMethod(subscriberNotifier, callbackMethod, nullptr, param);
    env->DeleteLocalRef(subscriberClass);
}

void sendNotification(JNIEnv *env, std::weak_ptr<jni::jobject_t> subscriberRefPtr){
    auto subscriberRef = subscriberRefPtr.lock();
    if (!subscriberRef)
        return;

    callJavaSendNotification(env, subscriberRef.get(), nullptr);
}
