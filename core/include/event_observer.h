#ifndef EVENT_OBSERVER_H
#define EVENT_OBSERVER_H

#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <map>

#define EventHandler(Sender, Event) std::shared_ptr<typename decltype(std::declval<Sender>().Event)::observer_callable_t>
#define MakeHandler(Sender, Event, Callable) std::make_shared<typename decltype(std::declval<Sender>().Event)::observer_callable_t>(Callable)

template <class Sender, typename... T>
class EventObserver{
public:
    EventObserver() = default;
    EventObserver(const EventObserver &) = delete;
    EventObserver &operator=(const EventObserver &) = delete;
    ~EventObserver();

    using observer_callable_t = std::function<void(const Sender &, T...)>;

    const EventObserver<Sender, T...> &operator+=(std::shared_ptr<observer_callable_t>);
    const EventObserver<Sender, T...> &operator-=(std::shared_ptr<observer_callable_t>);

private:
    enum class observer_request_type{
        observer_add,
        observer_remove
    };
    using observer_request_data = std::pair<std::shared_ptr<observer_callable_t>, observer_request_type>;
    friend Sender;
    void operator()(const Sender &, T...);
    void reset();
    void pendingSubscribersQueueHandlerFunc();
    std::map<observer_callable_t *, std::weak_ptr<observer_callable_t>> observerCollection;
    std::mutex observerCollectionMutex;
    std::queue<observer_request_data> pendingRequests;
    std::mutex pendingSubscribersQueueMutex;
    std::condition_variable pendingSubscribersQueueCondition;
    std::atomic<bool> pendingSubscribersHandlerStop{false};
    std::thread pendingSubscribersHandlerThread{&EventObserver<Sender, T...>::pendingSubscribersQueueHandlerFunc, this};
};

template <class Sender, typename... T>
EventObserver<Sender, T...>::~EventObserver() {
    std::unique_lock<std::mutex> pendingSubscribersLock(pendingSubscribersQueueMutex);
    pendingSubscribersHandlerStop.store(true);
    pendingSubscribersQueueCondition.notify_all();
    pendingSubscribersLock.unlock();
    pendingSubscribersHandlerThread.join();
}

template <class Sender, typename... T>
void EventObserver<Sender, T...>::operator()(const Sender &sender, T... params){

    std::unique_lock<std::mutex> observerCollectionCallLock(observerCollectionMutex);
    for (auto it = observerCollection.begin() ; it!= observerCollection.end();){
        auto observerCallablePtr = it->second;
        auto observer = observerCallablePtr.lock();
        if (observer) {
            observer->operator()(sender, params...);
            ++it;
        }
        else{
            it = observerCollection.erase(it);
        }
    }
}

template <class Sender, typename... T>
const EventObserver<Sender, T...> &EventObserver<Sender, T...>::operator+=(std::shared_ptr<observer_callable_t> observer){

    std::unique_lock<std::mutex> pendingSubscribersLock(pendingSubscribersQueueMutex);
    pendingRequests.push(observer_request_data(observer, observer_request_type::observer_add));
    pendingSubscribersQueueCondition.notify_all();
    return *this;
}


template <class Sender, typename... T>
void EventObserver<Sender, T...>::reset(){

    std::unique_lock<std::mutex> observerCollectionClearLock(observerCollectionMutex);
    observerCollection.clear();
}

template <class Sender, typename... T>
void EventObserver<Sender, T...>::pendingSubscribersQueueHandlerFunc() {
    while(!pendingSubscribersHandlerStop.load()){
        std::unique_lock<std::mutex> pendingSubscribersLock(pendingSubscribersQueueMutex);
        while(pendingRequests.size()>0){
            auto& observer = pendingRequests.front();
            if (observer.first.use_count() == 1)
                continue;

            std::unique_lock<std::mutex> observerCollectionLock(observerCollectionMutex);
            if (observer.second == observer_request_type::observer_add) {
                observerCollection[observer.first.get()] = observer.first;
            }
            else{
                observerCollection.erase(observer.first.get());
            }
            pendingRequests.pop();
        }
        pendingSubscribersQueueCondition.wait(pendingSubscribersLock);
    }
}

template <class Sender, typename... T>
const EventObserver<Sender, T...> &EventObserver<Sender, T...>::operator-=(std::shared_ptr<observer_callable_t> observer) {
    std::unique_lock<std::mutex> pendingSubscribersLock(pendingSubscribersQueueMutex);
    pendingRequests.push(observer_request_data(observer, observer_request_type::observer_remove));
    pendingSubscribersQueueCondition.notify_all();
    return *this;
}

#endif // EVENT_OBSERVER_H
