#ifndef EVENT_NOTIFIER_H
#define EVENT_NOTIFIER_H

#include <atomic>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <tuple>
#include <queue>
#include "event_listener.h"

namespace Neuro {

template<typename F, typename Tuple, size_t ...S >
decltype(auto) apply_tuple_impl(F&& fn, Tuple&& t, std::index_sequence<S...>){
  return std::forward<F>(fn)(std::get<S>(std::forward<Tuple>(t))...);
}

template<typename F, typename Tuple>
decltype(auto) apply_from_tuple(F&& fn, Tuple&& t){
  std::size_t constexpr tSize
    = std::tuple_size<typename std::remove_reference<Tuple>::type>::value;
  return apply_tuple_impl(std::forward<F>(fn),
                          std::forward<Tuple>(t),
                          std::make_index_sequence<tSize>());
}

template <typename R, typename... Args>
class Notifier {
public:
    using listener_t = Listener<R(Args...)>;
    using listener_ptr = ListenerPtr<R, Args...>;
    using listener_weak = ListenerWeakPtr<R, Args...>;

    Notifier():
        mNotifierThread([=](){notificationFunc();}){
    }
    Notifier(const Notifier &) = delete;
    Notifier& operator=(const Notifier &) = delete;
    ~Notifier(){
        mIsRunning.store(false);
        std::unique_lock<std::mutex> queueLock(mQueueMutex);
        mQueueEmptyCond.notify_one();
        if (mNotifierThread.joinable()){
            mNotifierThread.join();
        }
    }

    listener_ptr addListener(std::function<R(Args...)> callback){
        auto listenerPtr = std::make_shared<listener_t>(callback);
        mListenerCollection.push_back(listenerPtr);
        return listenerPtr;
    }

    void removeListener(listener_ptr listener){

    }

    void notifyAll(Args... args){
        if (mListenerCollection.size() == 0){
            return;
        }

        std::unique_lock<std::mutex> queueLock(mQueueMutex);
        mNotificationQueue.push(std::make_tuple<Args...>(std::forward<Args...>(args)...));
        mQueueEmptyCond.notify_one();
    }

private:
    static constexpr const char *class_name = "Notifier";

    std::queue<std::tuple<Args...>> mNotificationQueue;
    std::vector<listener_weak> mListenerCollection;
    std::atomic<bool> mIsRunning{true};
    std::mutex mQueueMutex;
    std::condition_variable mQueueEmptyCond;
    std::thread mNotifierThread;

    void notificationFunc(){
        while(mIsRunning.load()){
            std::unique_lock<std::mutex> queueLock(mQueueMutex);
            while(mNotificationQueue.size() == 0){
                if (!mIsRunning.load()){
                    return;
                }
                mQueueEmptyCond.wait(queueLock);
            }

            auto params = mNotificationQueue.front();
            for (auto listenerPtr : mListenerCollection){
                if (!mIsRunning.load()){
                    return;
                }
                auto listener = listenerPtr.lock();
                if (!listener){
                    continue;
                }
                apply_from_tuple([listener](auto pack){listener->notify(pack);}, params);
            }
            mNotificationQueue.pop();
        }
    }
};

}

#endif // EVENT_NOTIFIER_H
