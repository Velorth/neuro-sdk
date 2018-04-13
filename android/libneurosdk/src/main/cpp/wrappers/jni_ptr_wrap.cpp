#include "wrappers/jni_ptr_wrap.h"

void sendNotification(std::weak_ptr<jni::jobject_t> subscriberRefPtr){
    auto subscriberRef = subscriberRefPtr.lock();
    if (!subscriberRef)
        return;

    callJavaSendNotification(subscriberRef.get(), nullptr);
}
