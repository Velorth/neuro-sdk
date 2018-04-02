#ifndef SIGNAL_BUFFER_H
#define SIGNAL_BUFFER_H

#include "base_buffer.h"
#include "circular_buffer.h"

namespace Neuro {

template <typename SampleType, std::size_t BufferSize>
class SafeBuffer : public BaseBuffer<SampleType> {
public:
    void append(const std::vector<SampleType> &data) override {
        mBuffer.append(data);
        mTotalLength += data.size();
        mAvailableLength = mBuffer.dataLength();
    }

    std::vector<SampleType> readAvailable(std::size_t global_offset, std::size_t length) const override {
        if (length == 0){
            return std::vector<SampleType>();
        }

        if (mTotalLength == 0 || mAvailableLength == 0){
            return std::vector<SampleType>();
        }

        if (global_offset + length <= mTotalLength - mAvailableLength){
            return std::vector<SampleType>();
        }

        if (global_offset >= mTotalLength) {
            return std::vector<SampleType>();
        }

        auto bufferOffset = static_cast<long long>(global_offset)
                            - static_cast<long long>(mTotalLength)
                            + static_cast<long long>(mAvailableLength);

        if (bufferOffset < 0) {
            bufferOffset = 0;
        }

        auto readLength = length;
        decltype(readLength) trailingZerosCount{0};
        if (bufferOffset + readLength > mAvailableLength){
            trailingZerosCount = bufferOffset + readLength - mAvailableLength;
            readLength -= trailingZerosCount;
        }

        return mBuffer.read(bufferOffset, readLength);
    }

    std::vector<SampleType>
    readFill(std::size_t global_offset, std::size_t length, SampleType fill_value) const override {
        if (length == 0){
            return std::vector<SampleType>();
        }

        if (mTotalLength == 0 || mAvailableLength == 0){
            return std::vector<SampleType>(length, fill_value);
        }

        if (global_offset + length <= mTotalLength - mAvailableLength){
            return std::vector<SampleType>(length, fill_value);
        }

        if (global_offset >= mTotalLength) {
            return std::vector<SampleType>(length, fill_value);
        }

        auto bufferOffset = static_cast<long long>(global_offset)
                            - static_cast<long long>(mTotalLength)
                            + static_cast<long long>(mAvailableLength);
        decltype(bufferOffset) leadingZerosCount{0};
        if (bufferOffset < 0) {
            leadingZerosCount = -bufferOffset;
            bufferOffset = 0;
        }

        auto readLength = length;
        decltype(readLength) trailingZerosCount{0};
        if (bufferOffset + readLength > mAvailableLength){
            trailingZerosCount = mAvailableLength - bufferOffset - readLength;
            readLength -= trailingZerosCount;
        }

        std::vector<SampleType> resultBuffer(length);
        std::fill_n(resultBuffer.begin(), leadingZerosCount, fill_value);
        auto availableData = mBuffer.read(bufferOffset, readLength);
        std::copy(availableData.begin(), availableData.end(), resultBuffer.begin() + leadingZerosCount);
        std::fill(resultBuffer.end() - trailingZerosCount, resultBuffer.end(), fill_value);
        return resultBuffer;
    }

    std::vector<SampleType> rawBuffer() const override {
        return this->mBuffer.rawBuffer();
    }

    std::size_t availableLength() const noexcept override {
        return this->mAvailableLength;
    }

    std::size_t bufferSize() const noexcept override {
        return this->mBuffer.bufferSize();
    }

    std::size_t totalLength() const noexcept override {
        return this->mTotalLength;
    }

    void reset() override {
        mTotalLength = 0;
        mAvailableLength = 0;
        this->mBuffer.reset();
    }

private:
    static constexpr const char *class_name = "SafeBuffer";

    CircularBuffer<SampleType, BufferSize> mBuffer;
    std::size_t mAvailableLength{0};
    std::size_t mTotalLength{0};
    //std::function<>
};

}
#endif // SIGNAL_BUFFER_H
