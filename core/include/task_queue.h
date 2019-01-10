#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <functional>
#include <memory>
#include <string>
#include "lib_export.h"

namespace Neuro {

class SDK_SHARED TaskQueue final{
public:
    TaskQueue(const std::string &name = std::string());
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue& operator=(const TaskQueue &) = delete;
	TaskQueue(TaskQueue &&) noexcept;
	TaskQueue& operator=(TaskQueue &&) noexcept;
    ~TaskQueue();

    void exec(const std::function<void()> &);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}

#endif // TASK_QUEUE_H
