#ifndef EVENT_NOTIFIER_H
#define EVENT_NOTIFIER_H

#include <memory>
#include <vector>
#include <string>
#include "event_listener.h"
#include "task_queue.h"

namespace Neuro {

template <typename R, typename... Args>
class Notifier {
public:
    using listener_t = Listener<R(Args...)>;
    using listener_ptr = ListenerPtr<R, Args...>;
    using listener_weak = ListenerWeakPtr<R, Args...>;

    Notifier(std::string name = std::string()):
        mName(name){}

	Notifier(Notifier &&rhs) noexcept = default;
	Notifier& operator=(Notifier &&rhs)noexcept = default;

    listener_ptr addListener(std::function<R(Args...)> callback){
        auto listenerPtr = std::make_shared<listener_t>(callback);
        mListenerCollection.push_back(listenerPtr);
        return listenerPtr;
    }

    void notifyAll(Args... args){
        if (mListenerCollection.size() == 0){
            return;
        }
        mNotificationQueue.exec([=](){
            for (auto listenerPtr : mListenerCollection){
                auto listener = listenerPtr.lock();
                if (!listener){
                    continue;
                }
                listener->notify(args...);
            }
        });
    }

private:
    static constexpr const char *class_name = "Notifier";
    std::string mName;
    std::vector<listener_weak> mListenerCollection;
    TaskQueue mNotificationQueue{std::string("Notification") + std::string(" ") + mName};
};

}

#endif // EVENT_NOTIFIER_H
