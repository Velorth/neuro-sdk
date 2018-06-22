#include "device/callibri/callibri_buffer_collection.h"

namespace Neuro {

CallibriSignalBuffer &CallibriBufferCollection::signalBuffer(){
    if (!hasSignalBuffer()){
        throw std::runtime_error("Device does not have signal buffer");
    }
    return *mSignalBuffer;
}

CallibriRespirationBuffer &CallibriBufferCollection::respirationBuffer(){
    if (!hasRespirationBuffer()){
        throw std::runtime_error("Device does not have respiration buffer");
    }
    return *mRespirationBuffer;
}

CallibriMemsBuffer &CallibriBufferCollection::memsBuffer(){
    if (!hasMemsBuffer()){
        throw std::runtime_error("Device does not have MEMS buffer");
    }
    return *mMemsBuffer;
}


CallibriOrientationBuffer &CallibriBufferCollection::orientationBuffer(){
    if (!hasOrientationBuffer()){
        throw std::runtime_error("Device does not have angle buffer");
    }
    return *mOrientationBuffer;
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

void CallibriBufferCollection::setOrientationBuffer(std::unique_ptr<CallibriOrientationBuffer> &&buffer){
    mOrientationBuffer = std::move(buffer);
}

bool CallibriBufferCollection::hasSignalBuffer() const noexcept {
    return mSignalBuffer != nullptr;
}

bool CallibriBufferCollection::hasRespirationBuffer() const noexcept {
    return mRespirationBuffer != nullptr;
}

bool CallibriBufferCollection::hasMemsBuffer() const noexcept {
    return mMemsBuffer != nullptr;
}

bool CallibriBufferCollection::hasOrientationBuffer() const noexcept {
    return mOrientationBuffer != nullptr;
}

std::size_t CallibriBufferCollection::packetsLost(){
    return mSignalBuffer->packetsLost() +
        mRespirationBuffer->packetsLost() +
        mMemsBuffer->packetsLost() +
        mOrientationBuffer->packetsLost();
}

std::size_t CallibriBufferCollection::packetsReceived(){
    return mSignalBuffer->packetsReceived() +
        mRespirationBuffer->packetsReceived() +
        mMemsBuffer->packetsReceived() +
        mOrientationBuffer->packetsReceived();
}


}
