#ifndef EVENT_NOTIFIER_H
#define EVENT_NOTIFIER_H

#include <memory>
#include <vector>
#include "event_listener.h"

namespace Neuro {

template <typename R, typename... Args>
class Notifier {
public:
    using listener_t = Listener<R(Args...)>;
    using listener_ptr = ListenerPtr<R, Args...>;
    using listener_weak = ListenerWeakPtr<R, Args...>;

    listener_ptr addListener(std::function<R(Args...)> callback){
        auto listenerPtr = std::make_shared<listener_t>(callback);
        mListenerCollection.push_back(listenerPtr);
        return listenerPtr;
    }

    void removeListener(listener_ptr listener){

    }

    void notifyAll(Args... args){
        for (auto listenerPtr : mListenerCollection){
            auto listener = listenerPtr.lock();
            if (!listener){
                continue;
            }
            listener->notify(args...);
        }
    }

private:
    std::vector<listener_weak> mListenerCollection;
};

}

#endif // EVENT_NOTIFIER_H
