#ifndef LOOP_H
#define LOOP_H

#include <atomic>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "gsl/gsl_assert"
#include "logger.h"

namespace Neuro {

template <typename>
class Loop;

template <typename R, typename... Args>
class Loop<R(Args...)>{
public:
    using delay_time_t = std::chrono::duration<double>;

    template <typename Callable>
    Loop(Callable callable, delay_time_t delay, Args... args):
        mCallable(callable),
        mDelay(delay){
        Expects(mCallable != nullptr);
        mLoopThread = std::thread([=](){
            while (mThreadRunning.load()) {
                auto startTime = std::chrono::high_resolution_clock::now();
				try {
					mCallable(args...);
				}
				catch (...) {
					LOG_ERROR("Exception occured in user-provided loop function");
				}
                auto stopTime = std::chrono::high_resolution_clock::now();
				try {
					auto execTime = std::chrono::duration_cast<delay_time_t>(stopTime - startTime);
					if (execTime < delay) {
						LOG_TRACE_V("Sleep time: %f, exec time: %f", delay - execTime, execTime);
						std::unique_lock<std::mutex> waitLock(mWaitMutex);
						mWaitCondition.wait_for(waitLock, delay - execTime, [=] {return !mThreadRunning.load(); });
					}
				}
				catch (...) {
					LOG_ERROR("Exception occured in time-wait section");
				}
            }
        });
    }

    Loop(const Loop &) = delete;
    Loop& operator=(const Loop &) = delete;

    ~Loop(){
        LOG_TRACE("Loop destructor join");
        mThreadRunning.store(false);
        std::unique_lock<std::mutex> waitLock(mWaitMutex);
        mWaitCondition.notify_all();
        waitLock.unlock();
        try{
            if (mLoopThread.joinable())
                mLoopThread.join();
        }
        catch (std::system_error &e){
            LOG_ERROR_V("Thread destruction error: %s", e.what());
        }
        LOG_TRACE("Loop thread exited");
    }

    void setDelay(delay_time_t delay){
        mDelay = delay;
    }

private:
    static constexpr const char *class_name = "Loop";
    std::function<R(Args...)> mCallable;
    delay_time_t mDelay;
    std::atomic<bool> mThreadRunning{true};
    std::thread mLoopThread;
    std::mutex mWaitMutex;
    std::condition_variable mWaitCondition;
};

}

#endif // LOOP_H
