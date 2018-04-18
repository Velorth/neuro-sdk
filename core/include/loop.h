#ifndef LOOP_H
#define LOOP_H

#include <atomic>
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
                mCallable(args...);
                std::this_thread::sleep_for(delay);
            }
        });
    }

    Loop(const Loop &) = delete;
    Loop& operator=(const Loop &) = delete;

    ~Loop(){
        mThreadRunning.store(false);
        try{
            if (mLoopThread.joinable())
                mLoopThread.join();
        }
        catch (std::system_error &e){
            LOG_ERROR_V("Thread destruction error: %s", e.what());
        }
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
};

}

#endif // LOOP_H
