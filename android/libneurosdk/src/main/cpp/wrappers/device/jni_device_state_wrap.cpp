#include "wrappers/jni_device_state_wrap.h"

template<>
template<>
jni::java_object<Neuro::DeviceState>::java_object(const Neuro::DeviceState &deviceState):
        nativeObj(deviceState) {
    JNIEnv *env;
    auto resCode = get_env(&env);
    if (resCode == 2) return;

    auto fromIntCodeMethod = env->GetStaticMethodID(object_class, "fromIntCode",
                                                    jni::constructor_signature<Neuro::DeviceState>());
    javaObj = make_global_ref_ptr(env->CallStaticObjectMethod(object_class, fromIntCodeMethod,
                                                              static_cast<int>(nativeObj)));

    if (resCode == 1) detach_thread();
}

