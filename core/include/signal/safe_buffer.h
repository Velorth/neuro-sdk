#ifndef SAFE_BUFFER_H
#define SAFE_BUFFER_H

#include <mutex>
#include "gsl/gsl_assert"
#include "base_buffer.h"
#include "unbounded_buffer.h"

namespace Neuro {

template <typename SampleType, std::size_t BufferSize>
class SafeBuffer final : public BaseBuffer<SampleType> {
public:
    using length_callback_t = std::function<void(data_length_t)>;
    using length_listener_ptr = ListenerPtr<void, data_length_t>;

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) const noexcept override{
        return mBuffer.subscribeLengthChanged(callback);
    }

    void append(const std::vector<SampleType> &data) override {
        std::unique_lock<std::mutex> readLock(mReadMutex);
#ifndef NDEBUG
        auto totalLengthBefore = mBuffer.totalLength();
        auto expectedTotalLength = totalLengthBefore + data.size();
#endif
        mBuffer.append(data);
#ifndef NDEBUG
        Ensures(mBuffer.totalLength() == expectedTotalLength);
#endif
    }

    std::vector<SampleType> readAvailable(std::size_t global_offset, std::size_t length) const override {
        std::unique_lock<std::mutex> readLock(mReadMutex);
        return mBuffer.readAvailable(global_offset, length);
    }

    std::vector<SampleType>
    readFill(std::size_t global_offset, std::size_t length, SampleType fill_value) const override {
        std::unique_lock<std::mutex> readLock(mReadMutex);
        return mBuffer.readFill(global_offset, length, fill_value);
    }

    std::vector<SampleType> rawBuffer() const override {
        std::unique_lock<std::mutex> readLock(mReadMutex);
        return mBuffer.rawBuffer();
    }

    constexpr std::size_t bufferSize() const noexcept override {
        return mBuffer.bufferSize();
    }

    std::size_t totalLength() const noexcept override {
        std::unique_lock<std::mutex> readLock(mReadMutex);
        return mBuffer.totalLength();
    }

    void reset() override {        
        std::unique_lock<std::mutex> readLock(mReadMutex);
        mBuffer.reset();
    }

private:
    static constexpr const char *class_name = "SafeBuffer";

    UnboundedBuffer<SampleType, BufferSize> mBuffer;
    mutable std::mutex mReadMutex;
};

}
#endif // SIGNAL_BUFFER_H
