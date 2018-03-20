#include "wrappers/jni_ptr_wrap.h"

void callJavaSendNotification(jobject subscriberNotifier, jobject param){
    jni::call_in_attached_thread([=](auto env) {
        auto subscriberClass = env->GetObjectClass(subscriberNotifier);
        auto callbackMethod = env->GetMethodID(subscriberClass, "sendNotification",
                                               "(Ljava/lang/Object;Ljava/lang/Object;)V");
        env->CallVoidMethod(subscriberNotifier, callbackMethod, nullptr, param);
    });
}

void sendNotification(std::weak_ptr<jni::jobject_t> subscriberRefPtr){
    auto subscriberRef = subscriberRefPtr.lock();
    if (!subscriberRef)
        return;

    callJavaSendNotification(subscriberRef.get(), nullptr);
}
