#include "gsl/gsl_assert"
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include "task_queue.h"
#include "logger.h"

namespace Neuro {

struct TaskQueue::Impl final {
	static constexpr const char *class_name = "TaskQueue";
	const std::string mName;
	std::queue<std::function<void()>> mExecutionQueue;
	std::atomic<bool> mIsRunning{ true };
	std::mutex mQueueMutex;
	std::condition_variable mQueueCondition;
	std::thread mExecThread;

	Impl(const std::string &name):
		mName(name), 
		mExecThread([=]() { execFunction(); }) {
		Ensures(mExecThread.joinable()); 
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
			LOG_ERROR_V("Thread destruction error in queue <%s>: %s", mName.c_str(), e.what());
		}
	}

	void execFunction() {
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
			LOG_TRACE_V("Tasks in queue <%s> left %zd", mName.c_str(), mExecutionQueue.size());
			queueLock.unlock();

			try {
				task();
			}
			catch (std::exception &e) {
				LOG_ERROR_V("Task function in queue <%s> raised exception: %s", mName.c_str(), e.what());
			}
			catch (...) {
				LOG_ERROR_V("Task function in queue <%s> raised unknown exception", mName.c_str());
			}
		}
	}
};

TaskQueue::TaskQueue(const std::string &name):
	mImpl(std::make_unique<Impl>(name)){
}

TaskQueue::~TaskQueue() = default;

void TaskQueue::exec(const std::function<void()> &task_function){
    std::unique_lock<std::mutex> queueLock(mImpl->mQueueMutex);
	mImpl->mExecutionQueue.push(task_function);
	mImpl->mQueueCondition.notify_one();
}

}
