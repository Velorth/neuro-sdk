#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "java_helper.h"


GlobalClassRef::GlobalClassRef(JNIEnv *env, jclass java_class):
        mEnv(env),
        mClassRef(static_cast<jclass>(mEnv->NewGlobalRef(java_class))){}

GlobalClassRef::~GlobalClassRef() {
    mEnv->DeleteGlobalRef(mClassRef);
}

GlobalClassRef::operator jclass() const{
    return mClassRef;
}

void emplace_class_ref(JNIEnv *env, std::unordered_map<std::string, GlobalClassRef> &map_to_emplace, std::string class_name){
    auto classRef = env->FindClass(class_name.c_str());
    if (classRef == nullptr){
        __android_log_print(ANDROID_LOG_ERROR, "ClassMap", "Class %s not found", class_name.c_str());
        return;
    }
    map_to_emplace.try_emplace(class_name, classRef);
}

auto create_classes_map(JNIEnv *env){
    std::unordered_map<std::string, GlobalClassRef> classReferences;
    emplace_class_ref(env, classReferences, "com/neuromd/neurosdk/channels/ChannelInfo");
    emplace_class_ref(env, classReferences, "com/neuromd/neurosdk/channels/ChannelType");
    emplace_class_ref(env, classReferences, "com/neuromd/neurosdk/parameters/Command");
    return classReferences;
};

SdkJavaClasses::SdkJavaClasses(JNIEnv *env):
        mClasses(create_classes_map(env)) {}

const GlobalClassRef& SdkJavaClasses::fromClassName(const std::string &name) const {
    return mClasses.at(name);
}

struct AttachedTaskQueue::Impl final {
    std::queue<std::function<void(JNIEnv *)>> mExecutionQueue;
    std::atomic<bool> mIsRunning{ true };
    std::mutex mQueueMutex;
    std::condition_variable mQueueCondition;
    std::thread mExecThread;

    Impl(JNIEnv *env):
            mExecThread([=]() { execFunction(env); }) {
        if(!mExecThread.joinable()) {
            throw std::runtime_error("Task execution thread is not joinable");
        }
    }

    ~Impl() {
        mIsRunning.store(false);
        std::unique_lock<std::mutex> queueLock(mQueueMutex);
        mQueueCondition.notify_all();
        queueLock.unlock();
        try {
            if (mExecThread.joinable())
                mExecThread.join();
        }
        catch (std::system_error &e) {
            __android_log_print(ANDROID_LOG_ERROR, "AttachedTaskQueue", "Thread destruction error: %s", e.what());
        }
    }

    void execFunction(JNIEnv *env) {
        JavaVM *jvm;
        if (env->GetJavaVM(&jvm) != 0){
            __android_log_print(ANDROID_LOG_ERROR, "AttachedTaskQueue", "Failed to acquire jvm pointer");
            return;
        }
        auto attachResult = jvm->AttachCurrentThread(&env, nullptr);
        if (attachResult == 2){
            __android_log_print(ANDROID_LOG_ERROR, "AttachedTaskQueue", "Failed to attach native thread");
            return;
        }

        while (mIsRunning.load()) {
            std::unique_lock<std::mutex> queueLock(mQueueMutex);
            while (mExecutionQueue.empty()) {
                if (!mIsRunning.load()) {
                    return;
                }
                mQueueCondition.wait(queueLock);
            }
            auto task = mExecutionQueue.front();
            mExecutionQueue.pop();
            queueLock.unlock();

            try {
                task(env);
            }
            catch (std::exception &e) {
                __android_log_print(ANDROID_LOG_ERROR, "AttachedTaskQueue", "Task function raised exception: %s", e.what());
            }
            catch (...) {
                __android_log_print(ANDROID_LOG_ERROR, "AttachedTaskQueue", "Task function raised unknown exception");
            }
        }

        if (attachResult == 1){
            jvm->DetachCurrentThread();
        }
    }
};

AttachedTaskQueue::AttachedTaskQueue(JNIEnv *env):
        mImpl(std::make_unique<Impl>(env)){
}

AttachedTaskQueue::AttachedTaskQueue(AttachedTaskQueue &&) noexcept = default;

AttachedTaskQueue& AttachedTaskQueue::operator=(AttachedTaskQueue &&) noexcept = default;

AttachedTaskQueue::~TaskQueue() = default;

void AttachedTaskQueue::exec(const std::function<void(JNIEnv *)> &task_function){
    std::unique_lock<std::mutex> queueLock(mImpl->mQueueMutex);
    mImpl->mExecutionQueue.push(task_function);
    mImpl->mQueueCondition.notify_one();
}

ChannelInfo channel_info_from_jobject(JNIEnv *env, jobject javaChannelInfo) {
    auto getTypeMethod = env->GetMethodID(env->GetObjectClass(javaChannelInfo), "getType", "()Lcom/neuromd/neurosdk/channels/ChannelType;");
    if (getTypeMethod == nullptr){
        throw std::runtime_error("Method getName was not found in class ChannelInfo");
    }

    auto getNameMethod = env->GetMethodID(env->GetObjectClass(javaChannelInfo), "getName", "()Ljava/lang/String;");
    if (getNameMethod == nullptr){
        throw std::runtime_error("Method getName was not found in class ChannelInfo");
    }

    auto getIndexMethod = env->GetMethodID(env->GetObjectClass(javaChannelInfo), "getIndex", "()J");
    if (getIndexMethod == nullptr){
        throw std::runtime_error("Method getIndex was not found in class ChannelInfo");
    }


    auto javaTypeEnum = env->CallObjectMethod(javaChannelInfo, getTypeMethod);
    auto channelType = enum_from_java_obj<ChannelType>(env, javaTypeEnum);

    auto javaNameString = static_cast<jstring>(env->CallObjectMethod(javaChannelInfo, getNameMethod));
    auto nameChars = env->GetStringUTFChars(javaNameString, 0);
    auto name = std::string(nameChars);
    env->ReleaseStringUTFChars(javaNameString, nameChars);

    auto channelIndex = env->CallLongMethod(javaChannelInfo, getIndexMethod);
    if (channelIndex < 0){
        throw std::runtime_error("Channel index has invalid value");
    }

    ChannelInfo nativeChannelInfo;
    nativeChannelInfo.type = channelType;
    strcpy(nativeChannelInfo.name, name.c_str());
    nativeChannelInfo.index = static_cast<size_t>(channelIndex);

    return nativeChannelInfo;
}

std::string getParamTypeName(Parameter param) {
    switch (param){
        case ParameterName:
        case ParameterAddress:
        case ParameterSerialNumber:
            return "String";
        case ParameterHardwareFilterState:
            return "Boolean";
        case ParameterStimulatorAndMAState:
            return "StimulatorDeviceState";
        case ParameterOffset:
            return "Byte";
        case ParameterExternalSwitchState:
            return "ExternalSwitchInput";
        case ParameterState:
            return "DeviceState";
        case ParameterFirmwareMode:
            return "FirmwareMode";
        case ParameterSamplingFrequency:
            return "SamplingFrequency";
        case ParameterGain:
            return "Gain";
        case ParameterADCInputState:
            return "ADCInput";
        case ParameterAccelerometerSens:
            return "AccelerometerSensitivity";
        case ParameterGyroscopeSens:
            return "GyroscopeSensitivity";
        case ParameterStimulatorParamPack:
            return "StimulationParams";
        case ParameterMotionAssistantParamPack:
            return "MotionAssistantParams";
        case ParameterFirmwareVersion:
            return "FirmwareVersion";
    }
}


