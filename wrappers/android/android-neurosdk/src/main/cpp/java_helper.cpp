#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include "java_helper.h"

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