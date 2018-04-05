#include "device/callibri/callibri_buffer_collection.h"

namespace Neuro {

CallibriSignalBuffer &CallibriBufferCollection::signalBuffer(){
    if (mSignalBuffer == nullptr){
        throw std::runtime_error("Device does not have signal buffer");
    }
    return *mSignalBuffer;
}

CallibriRespirationBuffer &CallibriBufferCollection::respirationBuffer(){
    if (mRespirationBuffer == nullptr){
        throw std::runtime_error("Device does not have respiration buffer");
    }
    return *mRespirationBuffer;
}

CallibriMemsBuffer &CallibriBufferCollection::memsBuffer(){
    if (mMemsBuffer == nullptr){
        throw std::runtime_error("Device does not have MEMS buffer");
    }
    return *mMemsBuffer;
}


CallibriAngleBuffer &CallibriBufferCollection::angleBuffer(){
    if (mAngleBuffer == nullptr){
        throw std::runtime_error("Device does not have angle buffer");
    }
    return *mAngleBuffer;
}

void CallibriBufferCollection::setSignalBuffer(std::unique_ptr<CallibriSignalBuffer> &&buffer){
    mSignalBuffer = std::move(buffer);
}

void CallibriBufferCollection::setRespirationBuffer(std::unique_ptr<CallibriRespirationBuffer> &&buffer){
    mRespirationBuffer = std::move(buffer);
}

void CallibriBufferCollection::setMemsBuffer(std::unique_ptr<CallibriMemsBuffer> &&buffer){
    mMemsBuffer = std::move(buffer);
}

void CallibriBufferCollection::setAngleBuffer(std::unique_ptr<CallibriAngleBuffer> &&buffer){
    mAngleBuffer = std::move(buffer);
}

std::size_t CallibriBufferCollection::packetsLost(){
    return mSignalBuffer->packetsLost() +
        mRespirationBuffer->packetsLost() +
        mMemsBuffer->packetsLost() +
        mAngleBuffer->packetsLost();
}

std::size_t CallibriBufferCollection::packetsReceived(){
    return mSignalBuffer->packetsReceived() +
        mRespirationBuffer->packetsReceived() +
        mMemsBuffer->packetsReceived() +
        mAngleBuffer->packetsReceived();
}


}
