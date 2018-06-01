#include "gsl/gsl_assert"
#include "task_queue.h"
#include "logger.h"

namespace Neuro {

TaskQueue::TaskQueue(std::string name):
    mName(name),
    mExecThread([=](){ execFunction(); }){
    Ensures(mExecThread.joinable());
}

TaskQueue::~TaskQueue(){
    mIsRunning.store(false);
    std::unique_lock<std::mutex> queueLock(mQueueMutex);
    mQueueCondition.notify_all();
    queueLock.unlock();
    try{
        if (mExecThread.joinable())
            mExecThread.join();
    }
    catch (std::system_error &e){
        LOG_ERROR_V("Thread destruction error in queue <%s>: %s", mName.c_str(), e.what());
    }
}

void TaskQueue::exec(std::function<void()> task_function){
    std::unique_lock<std::mutex> queueLock(mQueueMutex);
    mExecutionQueue.push(task_function);
    mQueueCondition.notify_one();
}

void TaskQueue::execFunction(){
    while(mIsRunning.load()){
        std::unique_lock<std::mutex> queueLock(mQueueMutex);
        while(mExecutionQueue.size() == 0){
            if (!mIsRunning.load()){
                return;
            }
            mQueueCondition.wait(queueLock);
        }
        auto task = mExecutionQueue.front();
        mExecutionQueue.pop();
        LOG_TRACE_V("Tasks in queue <%s> left %zd", mName.c_str(), mExecutionQueue.size());
        queueLock.unlock();

        try {
            task();
        }
        catch(std::exception &e){
            LOG_ERROR_V("Task function in queue <%s> raised exception: %s", mName.c_str(), e.what());
        }
        catch(...){
            LOG_ERROR_V("Task function in queue <%s> raised unknown exception", mName.c_str());
        }
    }
}

}
