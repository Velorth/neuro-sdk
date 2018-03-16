#include "wrappers/jni_ptr_wrap.h"

void callJavaSendNotification(jobject subscriberNotifier, jobject param){
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return;

    auto subscriberClass = env->GetObjectClass(subscriberNotifier);
    auto callbackMethod = env->GetMethodID(subscriberClass, "sendNotification",
                                           "(Ljava/lang/Object;Ljava/lang/Object;)V");
    env->CallVoidMethod(subscriberNotifier, callbackMethod, nullptr, param);

    if (resCode == 1) jni::detach_thread();
}

void sendNotification(std::weak_ptr<jni::jobject_t> subscriberRefPtr){
    auto subscriberRef = subscriberRefPtr.lock();
    if (!subscriberRef)
        return;

    callJavaSendNotification(subscriberRef.get(), nullptr);
}
