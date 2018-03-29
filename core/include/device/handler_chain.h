#ifndef PACKET_PROCESSOR_H
#define PACKET_PROCESSOR_H

#include <memory>
#include <vector>
#include "handler.h"

namespace Neuro {

template <typename Handler>
class HandlerChain final{
public:
    using handler_ptr = std::shared_ptr<Handler>;

    HandlerChain() = default;
    HandlerChain(const HandlerChain &) = delete;
    HandlerChain& operator=(const HandlerChain &) = delete;

    void addHandler(handler_ptr handler){
        if (mHandlerContainer.size() > 0){
            auto& lastHandlerPtr = mHandlerContainer.back();
            lastHandlerPtr->setNext(handler);
        }
        mHandlerContainer.push_back(handler);
    }

    void reset(){
        mHandlerContainer.clear();
    }

    bool process(const typename Handler::element_type &element) noexcept {
        if (mHandlerContainer.size() == 0)
            return false;

        auto first = *mHandlerContainer.begin();
        return first->process(element);
    }

private:
    std::vector<handler_ptr> mHandlerContainer;
};

}

#endif // PACKET_PROCESSOR_H
