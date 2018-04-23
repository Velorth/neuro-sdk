#include "device/brainbit/brainbit_resistance_buffer.h"

namespace Neuro {

BrainbitResistanceBuffer::BrainbitResistanceBuffer(){
    mInputBuffer.reserve(AveragingSamplesCount);
}

BrainbitResistanceBuffer::length_listener_ptr BrainbitResistanceBuffer::subscribeLengthChanged(BaseBuffer::length_callback_t callback) const noexcept{
    return mMainBuffer.subscribeLengthChanged(callback);
}

void BrainbitResistanceBuffer::append(const std::vector<resistance_sample_t> &data){
    std::unique_lock<std::mutex> mInputBufferLock(mInputBufferMutex);
    mInputBuffer.insert(mInputBuffer.end(), data.begin(), data.end());
    if (mInputBuffer.size() >= AveragingSamplesCount){
        mInputBufferLock.unlock();
        flushBufferData();
    }
}

std::vector<resistance_sample_t> BrainbitResistanceBuffer::readAvailable(std::size_t global_offset, std::size_t length) const {
    return mMainBuffer.readAvailable(global_offset, length);
}

std::vector<resistance_sample_t> BrainbitResistanceBuffer::readFill(std::size_t global_offset, std::size_t length, resistance_sample_t fill_value) const {
    return mMainBuffer.readFill(global_offset, length, fill_value);
}

std::vector<resistance_sample_t> BrainbitResistanceBuffer::rawBuffer() const {
    return mMainBuffer.rawBuffer();
}

std::size_t BrainbitResistanceBuffer::bufferSize() const noexcept {
    return mMainBuffer.bufferSize();
}

std::size_t BrainbitResistanceBuffer::totalLength() const noexcept {
    return mMainBuffer.totalLength();
}

void BrainbitResistanceBuffer::reset(){
    std::unique_lock<std::mutex> mInputBufferLock(mInputBufferMutex);
    mInputBuffer.clear();
    mInputBufferLock.unlock();
    mMainBuffer.reset();
}

void BrainbitResistanceBuffer::flushBufferData(){
    decltype(mInputBuffer) newBuffer;
    newBuffer.reserve(AveragingSamplesCount);

    std::unique_lock<std::mutex> mInputBufferLock(mInputBufferMutex);
    using std::swap;
    swap(newBuffer, mInputBuffer);
    mInputBufferLock.unlock();

    Expects(newBuffer.size() % ChannelsCount == 0);


    for (std::size_t i = 0; i < newBuffer.size(); i+= ChannelsCount){
        for (std::size_t j = 0; j < ChannelsCount; ++j){

        }
    }
}

}
