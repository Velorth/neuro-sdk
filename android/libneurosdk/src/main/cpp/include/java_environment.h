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

#ifndef ANDROID_JAVA_ENVIRONMENT_H
#define ANDROID_JAVA_ENVIRONMENT_H

#include <jni.h>
#include <memory>
#include <string>

namespace jni {

    using jobject_t = std::remove_reference<decltype(*std::declval<jobject>())>::type;

    extern JavaVM *java_machine;

    int get_env(JNIEnv **g_env);

    void detach_thread();

    class attached_env final{
    public:
        attached_env(){
            auto resCode = get_env(&mEnv);
            if (resCode == 2)
                throw std::runtime_error("Cannot retrieve JNI environment");
            needDetach = (resCode == 1);
        }

        ~attached_env(){
            if (needDetach){
                detach_thread();
            }
        }

        JNIEnv* env() const noexcept{
            return mEnv;
        }

    private:
        JNIEnv *mEnv;
        bool needDetach;
    };

    template <typename Callable>
    auto call_in_attached_thread(Callable func) {
        attached_env env;
        return func(env.env());
    }

    template <typename Exc>
    bool java_throw(JNIEnv *env, const char *java_exception, Exc &&native_exception){
        auto exceptionClass = env->FindClass("java/lang/UnsupportedOperationException");
        if (exceptionClass == nullptr) {
            return false;
        }
        return !env->ThrowNew(exceptionClass, native_exception.what());
    }

    void delete_global_ref(jobject ref) noexcept;

    std::shared_ptr<jobject_t> make_global_ref_ptr(jobject localRef);

    template<typename T>
    constexpr const char *java_class_name();

    template<typename T>
    constexpr const char *constructor_signature();

    template<>
    constexpr const char *java_class_name<int>() { return "java/lang/Integer"; };

    template<>
    constexpr const char *constructor_signature<int>() { return "(I)V"; };

    template<>
    constexpr const char *java_class_name<long>() { return "java/lang/Long"; };

    template<>
    constexpr const char *constructor_signature<long>() { return "(J)V"; };

    template<>
    constexpr const char *java_class_name<double>() { return "java/lang/Double"; };

    template<>
    constexpr const char *constructor_signature<double>() { return "(D)V"; };

    template<>
    constexpr const char *java_class_name<bool>() { return "java/lang/Boolean"; };

    template<>
    constexpr const char *constructor_signature<bool>() { return "(Z)V"; };

    template<typename T>
    class java_object;

    template<typename NativeObj>
    void initJavaObjClass(JNIEnv *env) {
        java_object<NativeObj>::object_class = static_cast<jclass>(env->NewGlobalRef(
                env->FindClass(jni::java_class_name<NativeObj>())));
    }

    inline std::string getClassName(JNIEnv *env, jclass clazz) {
        jclass clazzz = env->GetObjectClass(clazz);
        auto mid = env->GetMethodID(clazzz, "toString", "()Ljava/lang/String;");
        jstring strObj = (jstring) env->CallObjectMethod(clazz, mid);

        const char *str = env->GetStringUTFChars(strObj, NULL);
        std::string res(str);

        env->ReleaseStringUTFChars(strObj, str);

        return res;
    }

    template<typename NativeObj>
    jobject getEnumFieldRef(JNIEnv *env, const char *name) {
        auto enumSignature = std::string("L") + jni::java_class_name<NativeObj>() + ";";
        auto enumFieldID = env->GetStaticFieldID(java_object<NativeObj>::java_class(),
                                                 name,
                                                 enumSignature.c_str());
        auto enumField = env->GetStaticObjectField(java_object<NativeObj>::java_class(),
                                                   enumFieldID);
        return enumField;
    }

    template<typename T>
    class java_object {
    public:

        /**
         * Constructor for built-in types
         */
        template<typename U = T>
        java_object(const typename std::enable_if<!std::is_pointer<U>::value, U>::type &obj)
                : nativeObj(obj) {
            call_in_attached_thread([=](auto env) {
                auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                               constructor_signature<T>());
                javaObj = make_global_ref_ptr(
                        env->NewObject(object_class, objectClassConstructor, nativeObj));
            });
        }

        /**
         * Constructor for pointers
         */
        template<typename U = T>
        java_object(const typename std::enable_if<std::is_pointer<U>::value, U>::type &obj)
                : nativeObj(obj) {
            call_in_attached_thread([=](auto env){
                auto objectClassConstructor = env->GetMethodID(object_class, "<init>",
                                                               constructor_signature<T>());
                javaObj = make_global_ref_ptr(env->NewObject(object_class, objectClassConstructor,
                                                             reinterpret_cast<jlong>(nativeObj)));
            });
        }

        java_object(const java_object &jObj) = delete;

        java_object &operator=(java_object rhs) = delete;

        operator jobject() {
            return javaObj.get();
        }

        T nativeObject() {
            return nativeObj;
        }

        static jclass java_class() {
            return object_class;
        }

    private:
        friend void initJavaObjClass<T>(JNIEnv *env);

        static jclass object_class;
        T nativeObj;
        std::shared_ptr<jobject_t> javaObj;
    };

    template<typename T>
    jclass java_object<T>::object_class = nullptr;
}
#endif //ANDROID_JAVA_ENVIRONMENT_H
