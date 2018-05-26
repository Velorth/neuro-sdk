#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

namespace Neuro {

class TaskQueue{
public:
    using delay_time_t = std::chrono::duration<double>;

    TaskQueue(std::string name = std::string());
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue& operator=(const TaskQueue &) = delete;
    ~TaskQueue();

    void exec(std::function<void()>);

private:
    static constexpr const char *class_name = "TaskQueue";
    const std::string mName;
    std::queue<std::function<void()>> mExecutionQueue;
    std::atomic<bool> mIsRunning{true};
    std::thread mExecThread;
    std::mutex mQueueMutex;
    std::condition_variable mQueueCondition;

    void execFunction();
};

}

#endif // TASK_QUEUE_H
