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

#include <algorithm>
#include "signal/circular_buffer.h"
#include "signal/signal_reader.h"
#include "logger.h"
/*
using std::vector;
using std::mutex;
using std::unique_lock;

SimpleReader::SimpleReader(std::unique_ptr<CircularBuffer> signal_buffer):
        SignalReader(std::move(signal_buffer)){

}

vector<SignalSample> SimpleReader::readDataForChannel(int channel_index, size_t offset, size_t length) const {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Read length: %d", BRAINBIT_READER_CLASSNAME, __FUNCTION__, length);

    unique_lock<mutex> bufferLock(dataBufferMutex);

    //here we need to translate overall offset and length into buffer positions

    auto bufferDataLength = signalBuffer->getDataLength();
    auto overallLength = signalBuffer->getTotalAppendedDataLength();

    if (offset>=overallLength) return vector<SignalSample>();

    if (overallLength>bufferDataLength)
    {
        if (offset >= overallLength - bufferDataLength)
        {
            offset -= overallLength - bufferDataLength;
            if (offset+length<overallLength)
            {
                return signalBuffer->getData(offset, length);
            }
            else
            {
                length -= overallLength-(offset+length);
                return signalBuffer->getData(offset, length);
            }
        }
        else
        {
            if (offset + length > overallLength - bufferDataLength)
            {
                auto shift = overallLength - bufferDataLength - offset;
                if (offset+length>=overallLength){
                    length -= overallLength-(offset+length);
                }
                offset -= overallLength - bufferDataLength;
                std::vector<SignalSample> outVector(length);
                std::fill_n(outVector.begin(), shift, 0.0);
                auto readData = signalBuffer->getData(offset+shift, length-shift);
                std::copy(readData.begin(), readData.end(), outVector.begin()+shift);
                return outVector;
            }
            else{
                //here zeros must be loaded from fs
                std::vector<SignalSample> outVector(length);
                std::fill_n(outVector.begin(), length, 0.0);
                return outVector;
            }
        }
    }
    else
    {
        return signalBuffer->getData(offset, length);
    }
}

void SimpleReader::setSamplingFrequency(int frequency) {

    if (frequency < 0)
        return;

    if (signalBuffer &&
        signalBuffer->getBufferSize() == SIMPLE_BUFFER_DURATION * frequency) {
        return;
    }

    std::unique_lock<std::mutex> bufferLock(dataBufferMutex);
    signalBuffer.reset(new CircularBuffer(static_cast<size_t>(frequency * SIMPLE_BUFFER_DURATION)));
    bufferLock.unlock();

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Buffer recreated for size %zd", SIMPLE_READER_CLASSNAME, __FUNCTION__,
               signalBuffer->getBufferSize());
}

void SimpleReader::onDataReceived(const std::vector<SignalSample> &samples) {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Got %lu samples. Appending to buffer", SIMPLE_READER_CLASSNAME, __FUNCTION__, samples.size());

    auto bufferToAppend = samples;
    if(signalReverseNeeded.load()){
        std::transform(bufferToAppend.begin(), bufferToAppend.end(), bufferToAppend.begin(), [](SignalSample s)->SignalSample{return -s;});
    }
    std::unique_lock<std::mutex> bufferLock(dataBufferMutex);
    signalBuffer->append(bufferToAppend);
    auto bufferLength = signalBuffer->getTotalAppendedDataLength();
    bufferLock.unlock();
    log->trace("[%s: %s] Total appended to buffer: %d.", SIMPLE_READER_CLASSNAME, __FUNCTION__, bufferLength);

    this->setChangedLength(bufferLength);
}

void SimpleReader::onDataReceived(std::vector<SignalSample> &&samples) {

    std::vector<SignalSample> signal = std::move(samples);
    this->onDataReceived(signal);
}


*/



















