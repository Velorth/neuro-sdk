#ifndef PACKET_RESPONSIBILITY_H
#define PACKET_RESPONSIBILITY_H

#include <memory>
#include "common_types.h"
#include "logger.h"

namespace Neuro {

template <typename T>
class HandlerChain;

template <typename Element>
class Handler{
public:
    using element_type = Element;

    Handler() = default;
    Handler(const Handler &) = delete;
    Handler& operator=(const Handler &) = delete;
    virtual ~Handler() = default;

    bool process(const Element &element) noexcept {
        if (isResponsible(element)){
            try {
                return processElement(element);
            }
            catch (std::exception &e){
                LOG_WARN_V("Element processing failed: %s", e.what());
                return false;
            }
        }
        else {
            auto nextPtr = mNextResponsible.lock();
            if (nextPtr == nullptr){
                return false;
            }
            return nextPtr->process();
        }
    }

private:
    friend class HandlerChain<Handler<Element>>;

    static constexpr const char *class_name = "Handler";

    std::weak_ptr<Handler> mNextResponsible;

    virtual bool isResponsible(const element_type &) const noexcept = 0;
    virtual bool processElement(const element_type &) = 0;

    void setNext(std::weak_ptr<Handler> next){
        mNextResponsible = next;
    }
};

}

#endif // PACKET_RESPONSIBILITY_H
