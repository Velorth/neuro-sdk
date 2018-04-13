/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
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

#include <saturation_cast.h>
#include "signal/signal_reader.h"
#include "logger.h"
/*
BrainbitReader::BrainbitReader(std::unique_ptr<CircularBuffer> signal_buffer):
        SignalReader(std::move(signal_buffer)){}


std::vector<SignalSample> BrainbitReader::readDataForChannel(int channel_index, std::size_t offset, const std::size_t length) const {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Read offset: %d, length: %d", BRAINBIT_READER_CLASSNAME, __FUNCTION__, offset, length);

    auto rawSamplesCount = length * BRAINBIT_CHANNELS_COUNT;
    auto realOffset = offset*BRAINBIT_CHANNELS_COUNT;
    //Prevents cache from overwriting during reading from another thread
    //to exclude situations were we entered here with one time/duration
    //and got information for another time/duration from cache because of
    //another thread re-requested cache from ring buffer
    std::unique_lock<std::mutex> cacheLock(cacheMutex);
    //if we already read data for that timings, we can read channel data from cached buffer
    /*if (offset != lastOffset || length != lastLength
        || requestBuffer.empty() || requestBuffer.size() != rawSamplesCount)
    {
        //there is no cached data for given offset/length
        //so we read them from ring buffer

        requestBuffer.resize(rawSamplesCount);

        //Reading data from ring buffer
        std::unique_lock<std::mutex> dataBufferLock(dataBufferMutex);
        auto totalDataLength = signalBuffer->getTotalAppendedDataLength();
        auto bufferDataLength = signalBuffer->getDataLength();

        //Here we must load data from external storage, but now we just fill out buffer with zeros
        // because we have no external storage yet. It could be internet service or hard drive
        if (realOffset + rawSamplesCount > totalDataLength - bufferDataLength && realOffset < totalDataLength)
        {
            SignalSample *start = requestBuffer.data();
            auto count = rawSamplesCount;
            if (realOffset + rawSamplesCount > totalDataLength)
            {

                auto overflow = realOffset + rawSamplesCount - totalDataLength;
                count -= overflow;
                std::fill_n(start + rawSamplesCount - overflow, overflow, 0.0);
            }

            signalBuffer->getData(start, saturation_cast<uint32_t>(offset), count);
            log->debug("[%s: %s] Count %d", BRAINBIT_READER_CLASSNAME, __FUNCTION__, count);
        }
        else
        {
            log->trace("[%s: %s] Filling with zeros", BRAINBIT_READER_CLASSNAME, __FUNCTION__);
            std::fill_n(requestBuffer.data(), rawSamplesCount, 0.0);
        }
        dataBufferLock.unlock();
        lastOffset = offset;
        lastLength = length;
    }

    std::vector<SignalSample> outBuffer(length);
    //Parsing data from cached buffer to output
    for (std::size_t i = 0; i < length; ++i)
    {
        outBuffer[i] = (requestBuffer[i * BRAINBIT_CHANNELS_COUNT + channel_index]);
    }

    return outBuffer;
}

void BrainbitReader::setSamplingFrequency(int){

}

void BrainbitReader::onDataReceived(const std::vector<SignalSample> &data){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Got %lu samples. Appending to buffer", BRAINBIT_READER_CLASSNAME, __FUNCTION__, data.size());
    std::unique_lock<std::mutex> bufferLock(dataBufferMutex);
    signalBuffer->append(data);
    auto bufferLength = signalBuffer->getTotalAppendedDataLength();
    bufferLock.unlock();
    log->trace("[%s: %s] Total appended to buffer: %d.", BRAINBIT_READER_CLASSNAME, __FUNCTION__, bufferLength);

    this->setChangedLength(bufferLength);
}

void BrainbitReader::onDataReceived(std::vector<SignalSample> &&data){

    std::vector<SignalSample> signal = std::move(data);
    this->onDataReceived(signal);
}

std::size_t BrainbitReader::getOverallLength() const {
    return SignalReader::getOverallLength() / BRAINBIT_CHANNELS_COUNT;
}
*/



