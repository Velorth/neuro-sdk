/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neuromd.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <algorithm>
#include <array>
#include <vector>

template <typename SampleType, std::size_t BufferSize>
class CircularBuffer
{
public:
    CircularBuffer();
    CircularBuffer(const CircularBuffer &);
    CircularBuffer& operator=(const CircularBuffer &);
    CircularBuffer(CircularBuffer &&) = default;
    CircularBuffer& operator=(CircularBuffer &&) = default;

    void append(const std::vector<SampleType> &);
    std::vector<SampleType> read(std::size_t head_offset, std::size_t length) const;
    std::vector<SampleType> rawBuffer() const;
    std::size_t dataLength() const noexcept;
    constexpr std::size_t bufferSize() const noexcept;
    void reset();

private:
    using BufferType = std::array<SampleType, BufferSize>;
	using BufferIterator = typename BufferType::iterator;

    static constexpr const char *class_name = "CircularBuffer";

    BufferType mBufferArray;
    BufferIterator mHead;
	BufferIterator mTail;
	BufferIterator mBufferBegin{ mBufferArray.begin() };
    static constexpr std::size_t mBufferSize = BufferSize;
    std::size_t mDataLength{0};
};

template<typename SampleType, std::size_t BufferSize>
CircularBuffer<SampleType, BufferSize>::CircularBuffer():
    mHead(mBufferArray.begin()),
    mTail(mBufferArray.begin()){

}

template<typename SampleType, std::size_t BufferSize>
CircularBuffer<SampleType, BufferSize>::CircularBuffer(const CircularBuffer &rhs) :
    mBufferArray(rhs.mBufferArray),
    mHead(mBufferArray.begin() + (rhs.mHead - rhs.mBufferArray.begin())),
    mTail(mBufferArray.begin() + (rhs.mTail - rhs.mBufferArray.begin())),
    mDataLength(rhs.mDataLength){

}

template<typename SampleType, std::size_t BufferSize>
CircularBuffer<SampleType, BufferSize>&
CircularBuffer<SampleType, BufferSize>::operator=(const CircularBuffer &rhs) {
    mBufferArray = rhs.mBufferArray;
    mHead = mBufferArray.begin() + (rhs.mHead - rhs.mBufferArray.begin());
    mTail = mBufferArray.begin() + (rhs.mTail - rhs.mBufferArray.begin());
    mDataLength = rhs.mDataLength;
    return *this;
}

template<typename SampleType, std::size_t BufferSize>
void
CircularBuffer<SampleType, BufferSize>::append(const std::vector<SampleType> &dataContainer){
	const SampleType *data = dataContainer.data();
	auto length = dataContainer.size();
	if (length == 0)
		return;

	//if data length is greater than buffer maximum size
	//we should shift start position of data to write
	//last RING_BUFFER_SIZE samples from input array to buffer
	if (length > mBufferSize)
	{
		data += length - mBufferSize;
		length = mBufferSize;
	}

	auto newHead = mHead;
	auto newTailDistance = mDataLength > 0 ? std::distance(mBufferArray.begin(), mTail) + length : std::distance(mBufferArray.begin(), mTail) + length - 1;
	decltype(mTail) newTail;
	//Situation where appended data don't exceed buffer at end
	//Tail still is to the left from the end
	//Just adding data from old tail to new
	if (newTailDistance < mBufferArray.size())
	{
		std::copy(data, data + length, mDataLength > 0 ? mTail + 1 : mTail);
		newTail = mBufferArray.begin() + newTailDistance;
		//Now we must check were is old head relative to old tail
		//If head to the right from the tail it means that tail is
		//one position before head, because if tail exceeded buffer size
		//in the past, it's moved head to position after itself,
		//and now we have same situation, we must move head to position
		//1 sample after tail
		if (mTail < mHead)
		{
			newHead = newTail + 1;
		}
	}
	//In case when new tail exceeds buffer end position in memory,
	//we should move it to start of buffer with offset equals to
	//exceeded samples count.
	else
	{
		auto overflow = newTailDistance - mBufferArray.size() + 1;
		auto beforeBufferEnd = length - overflow;

		newTail = mBufferArray.begin() + overflow - 1;

		if (beforeBufferEnd > 0) {
			std::copy(data, data + beforeBufferEnd, mTail + 1);
		}

		std::copy(data + beforeBufferEnd, data + beforeBufferEnd + overflow, mBufferArray.begin());

		//Head can't be before tail in this case
		newHead = newTail + 1;
	}
	mTail = newTail;
	mHead = newHead;

	mDataLength += length;
	if (mDataLength > mBufferSize) mDataLength = mBufferSize;
}

template<typename SampleType, std::size_t BufferSize>
std::vector<SampleType>
CircularBuffer<SampleType, BufferSize>::rawBuffer() const {
    return std::vector<SampleType>(mBufferArray.begin(), mBufferArray.end());
}

template<typename SampleType, std::size_t BufferSize>
std::size_t
CircularBuffer<SampleType, BufferSize>::dataLength() const noexcept {
    return mDataLength;
}

template<typename SampleType, std::size_t BufferSize>
std::size_t
constexpr CircularBuffer<SampleType, BufferSize>::bufferSize() const noexcept {
    return mBufferSize;
}

template<typename SampleType, std::size_t BufferSize>
void CircularBuffer<SampleType, BufferSize>::reset(){
    mHead = mBufferArray.begin();
    mTail = mBufferArray.begin();
}

template<typename SampleType, std::size_t BufferSize>
std::vector<SampleType>
CircularBuffer<SampleType, BufferSize>::read(std::size_t head_offset, std::size_t length) const {
    std::vector<SampleType> outVector(length);

    if (head_offset >= mDataLength || head_offset + length > mDataLength){
        throw std::runtime_error("Offset and length are out of range");
    }

	auto readStartDistance = std::distance(mBufferBegin, mHead) + head_offset;
    if (readStartDistance >= mBufferArray.size()) {
        auto fromEndDistance = readStartDistance - mBufferArray.size() + 1;
        auto shiftedReadStart = mBufferArray.begin() + fromEndDistance - 1;
        std::copy(shiftedReadStart, shiftedReadStart+length, outVector.begin());
    }
    else {
        auto readEndDistance = readStartDistance + length - 1;
		auto readStart = mHead + head_offset;
        if (readEndDistance >= mBufferArray.size()) {
            auto fromEnd = readEndDistance - mBufferArray.size() + 1;
            auto beforeBufferEnd = length - fromEnd;
            std::copy(readStart, readStart+beforeBufferEnd, outVector.begin());
            std::copy(mBufferArray.begin(),
                      mBufferArray.begin() + fromEnd,
                      outVector.begin() + beforeBufferEnd);
        }
        else {
            std::copy(readStart, readStart+length, outVector.begin());
        }
    }

    return outVector;
}

#endif //CIRCULAR_BUFFER_H
