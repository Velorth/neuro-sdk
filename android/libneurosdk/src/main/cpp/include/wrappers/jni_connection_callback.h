#ifndef ANDROID_JNI_CONNECTION_CALLBACK_H
#define ANDROID_JNI_CONNECTION_CALLBACK_H

#include <string>
#include <android/log.h>
#include "jni_ptr_wrap.h"

template <class JniDevice>
void deviceFoundCallback(void *subscriber, typename JniDevice::object_ptr_t foundDevice) {
    JNIEnv *env;
    auto resCode = jni::get_env(&env);
    if (resCode == 2) return;

    auto device = new JniDevice(foundDevice);

    //creating java wrapper object for JniDevice
    jni::java_object<decltype(device)> deviceWrapObj(device);

    device->subscribeStateChanged(find_notifier<decltype(device)>(deviceWrapObj, "deviceStateChanged"));
    device->subscribeBatteryLevelChanged(find_notifier<decltype(device)>(deviceWrapObj, "batteryLevelChanged"));

    __android_log_print(ANDROID_LOG_VERBOSE, "DeviceConnectionJni",
                        "Device object created. Passing to java method");
    //pass wrapper object to callback function
    //subscriber must have onDeviceFound method with void(BfbDevice) signature
    auto subscriberClass = env->GetObjectClass((jobject) subscriber);
    auto callbackMethodSignature = std::string("(L") + jni::java_class_name<decltype(device)>() + ";)V";
    auto callbackMethod = env->GetMethodID(subscriberClass, "onDeviceFound", callbackMethodSignature.c_str());
    env->CallVoidMethod((jobject) subscriber, callbackMethod, static_cast<jobject>(deviceWrapObj));

    if (resCode == 1) jni::detach_thread();
}

#endif //ANDROID_JNI_CONNECTION_CALLBACK_H