#ifndef JAVA_HELPER_H
#define JAVA_HELPER_H

#include <functional>
#include <jni.h>
#include <android/log.h>
#include <string>
#include <map>
#include "sdk_error.h"
#include "cparams.h"
#include "cdevice.h"
#include "cchannels.h"

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

inline std::string get_class_name(JNIEnv *env, jclass clazz) {
    jclass clazzz = env->GetObjectClass(clazz);
    auto mid = env->GetMethodID(clazzz, "toString", "()Ljava/lang/String;");
    jstring strObj = (jstring) env->CallObjectMethod(clazz, mid);

    const char *str = env->GetStringUTFChars(strObj, NULL);
    std::string res(str);

    env->ReleaseStringUTFChars(strObj, str);
    env->DeleteLocalRef(strObj);

    return res;
}

static inline jobject get_enum_field_ref(JNIEnv *env, jclass enum_class, const char *name) {
    auto enumSignature = std::string("L") + get_class_name(env, enum_class) + ";";
    auto enumFieldID = env->GetStaticFieldID(enum_class,
                                             name,
                                             enumSignature.c_str());
    auto enumField = env->GetStaticObjectField(enum_class,
                                               enumFieldID);
    return enumField;
}

template <typename NativeEnum>
class enum_name_map{
public:
    enum_name_map() = delete;

    static std::string name(NativeEnum value) {
        return mEnumToNameMap.at(value);
    }
    static NativeEnum value(std::string name) {
        return mNameToEnumMap.at(name);
    }
private:
    static const std::map<NativeEnum, std::string> mEnumToNameMap;
    static const std::map<std::string, NativeEnum> mNameToEnumMap;
};

template <typename Enum>
Enum enum_from_java_obj(JNIEnv *env, jobject enum_obj){
    auto commandEnumClass = env->GetObjectClass(enum_obj);
    auto nameMethodID = env->GetMethodID(commandEnumClass, "name", "()Ljava/lang/String;");
    auto nameString = static_cast<jstring>(env->CallObjectMethod(enum_obj, nameMethodID));
    auto enumName = env->GetStringUTFChars(nameString, nullptr);
    auto enumVal = enum_name_map<Enum>::value(enumName);
    env->ReleaseStringUTFChars(nameString, enumName);
    env->DeleteLocalRef(nameString);
    return enumVal;
}

template<>
const std::map<DeviceState, std::string>
        enum_name_map<DeviceState>::mEnumToNameMap;

template<>
const std::map<std::string, DeviceState>
        enum_name_map<DeviceState>::mNameToEnumMap;

template<>
const std::map<Command, std::string>
        enum_name_map<Command>::mEnumToNameMap;

template<>
const std::map<std::string, Command>
        enum_name_map<Command>::mNameToEnumMap;

template<>
const std::map<Parameter, std::string>
        enum_name_map<Parameter>::mEnumToNameMap;

template<>
const std::map<std::string, Parameter>
        enum_name_map<Parameter>::mNameToEnumMap;

template<>
const std::map<ParamAccess, std::string>
        enum_name_map<ParamAccess>::mEnumToNameMap;

template<>
const std::map<std::string, ParamAccess>
        enum_name_map<ParamAccess>::mNameToEnumMap;

template<>
const std::map<ADCInput, std::string>
        enum_name_map<ADCInput>::mEnumToNameMap;

template<>
const std::map<std::string, ADCInput>
        enum_name_map<ADCInput>::mNameToEnumMap;

template<>
const std::map<AccelerometerSensitivity , std::string>
        enum_name_map<AccelerometerSensitivity>::mEnumToNameMap;

template<>
const std::map<std::string, AccelerometerSensitivity >
        enum_name_map<AccelerometerSensitivity>::mNameToEnumMap;


template<>
const std::map<GyroscopeSensitivity , std::string>
        enum_name_map<GyroscopeSensitivity>::mEnumToNameMap;

template<>
const std::map<std::string, GyroscopeSensitivity >
        enum_name_map<GyroscopeSensitivity>::mNameToEnumMap;

template<>
const std::map<ExternalSwitchInput, std::string>
        enum_name_map<ExternalSwitchInput>::mEnumToNameMap;

template<>
const std::map<std::string, ExternalSwitchInput>
        enum_name_map<ExternalSwitchInput>::mNameToEnumMap;


template<>
const std::map<FirmwareMode, std::string>
        enum_name_map<FirmwareMode>::mEnumToNameMap;

template<>
const std::map<std::string, FirmwareMode>
        enum_name_map<FirmwareMode>::mNameToEnumMap;


template<>
const std::map<Gain, std::string>
        enum_name_map<Gain>::mEnumToNameMap;

template<>
const std::map<std::string, Gain>
        enum_name_map<Gain>::mNameToEnumMap;


template<>
const std::map<MotionAssistantLimb, std::string>
        enum_name_map<MotionAssistantLimb>::mEnumToNameMap;

template<>
const std::map<std::string, MotionAssistantLimb>
        enum_name_map<MotionAssistantLimb>::mNameToEnumMap;




template<>
const std::map<StimulatorAndMaState, std::string>
        enum_name_map<StimulatorAndMaState>::mEnumToNameMap;

template<>
const std::map<std::string, StimulatorAndMaState>
        enum_name_map<StimulatorAndMaState>::mNameToEnumMap;



template<>
const std::map<SamplingFrequency, std::string>
        enum_name_map<SamplingFrequency>::mEnumToNameMap;

template<>
const std::map<std::string, SamplingFrequency>
        enum_name_map<SamplingFrequency>::mNameToEnumMap;


template<typename T, typename F>
inline jobjectArray to_obj_array(JNIEnv *env, jclass element_class, const char *constructor_signature, T *array, jsize size, F creation_func) {
    auto objArray = env->NewObjectArray(size, element_class, nullptr);

    for (auto i = 0 ; i < size; ++i) {
        env->PushLocalFrame(1);
        auto constructor = env->GetMethodID(element_class, "<init>", constructor_signature);
        auto objLocalRef = creation_func(env, element_class, constructor, array[i]);
        env->SetObjectArrayElement(objArray, i, objLocalRef);
        env->PopLocalFrame(nullptr);
    }
    return objArray;
}

std::string getParamTypeName(Parameter param);

#endif