#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <functional>
#include <memory>

namespace Neuro {

template <typename>
class Listener;

template <typename R, typename... Args>
class Listener<R(Args...)> {
public:
    Listener(std::function<R(Args...)> func):
        mFunc(func){
    }

    Listener(const Listener &) = delete;
    Listener& operator=(const Listener &) = delete;

private:
    template <typename, typename... > friend class Notifier;
    std::function<R(Args...)> mFunc;
    void notify(Args... args){
        mFunc(args...);
    }
};

template <typename R, typename... Args>
using ListenerPtr = std::shared_ptr<Listener<R(Args...)>>;

template <typename R, typename... Args>
using ListenerWeakPtr = std::weak_ptr<Listener<R(Args...)>>;

}

#endif // EVENT_LISTENER_H
