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

#ifndef SIGNAL_READER_H
#define SIGNAL_READER_H

#include "circular_buffer.h"
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>


#define BRAINBIT_CHANNELS_COUNT 4
#define BRAINBIT_BUFFER_DURATION 60 //in seconds
#define BRAINBIT_SAMPLE_FREQ 250
#define BRAINBIT_READER_CLASSNAME "BrainbitReader"

#define SIMPLE_BUFFER_DURATION 60u
#define SIMPLE_READER_CLASSNAME "SimpleSignalReader"

/*
class SignalReader
{
public:
    SignalReader(std::unique_ptr<CircularBuffer<double, SIMPLE_BUFFER_DURATION>> signal_buffer);
    virtual ~SignalReader();
    virtual std::vector<double> readDataForChannel(int channel_index, size_t offset, size_t length) const = 0;
    virtual void setSamplingFrequency(int) = 0;
    virtual void onDataReceived(const std::vector<SignalSample> &) = 0;
    virtual void onDataReceived(std::vector<SignalSample> &&) = 0;
    void subscribeSignalLengthChanged(std::function<void(size_t)>) const;
    size_t getBufferSize() const;
    void setSignalBuffer(std::unique_ptr<CircularBuffer>);
    void setSignalInverse(bool);
    virtual size_t getOverallLength() const;
protected:
    mutable std::function<void(size_t)> signalLengthChangedCallback;
    std::unique_ptr<CircularBuffer> signalBuffer;
    mutable std::mutex dataBufferMutex;
    std::atomic<bool> signalReverseNeeded{false};

    void setChangedLength(std::size_t);
private:
    std::size_t changedLength;
    std::mutex notificationThreadMutex;
    std::condition_variable lengthChangedCondition;
    std::atomic<bool> threadStop;
    std::thread lengthChangedNotificationThread;

    void lengthChangedNotificationFunc();
};

class BrainbitReader: public SignalReader
{
public:
    BrainbitReader(std::unique_ptr<CircularBuffer> signal_buffer);
    BrainbitReader(const BrainbitReader &) = delete;
    BrainbitReader& operator=(const BrainbitReader &) = delete;
    ~BrainbitReader() = default;
    std::vector<SignalSample> readDataForChannel(int channel_index, size_t offset, size_t length) const override;
    void setSamplingFrequency(int) override;
    void onDataReceived(const std::vector<SignalSample> &) override;
    void onDataReceived(std::vector<SignalSample> &&) override;
    std::size_t getOverallLength() const override;
private:
    mutable std::vector<SignalSample> requestBuffer;
    mutable std::mutex cacheMutex;
    mutable std::size_t lastOffset;
    mutable std::size_t lastLength;
};


class SimpleReader: public SignalReader
{
public:
    SimpleReader(std::unique_ptr<CircularBuffer> signal_buffer);
    SimpleReader(const SimpleReader &) = delete;
    SimpleReader& operator=(const SimpleReader &) = delete;
    ~SimpleReader() = default;
    std::vector<SignalSample> readDataForChannel(int channel_index, size_t offset, size_t length) const override;
    void setSamplingFrequency(int) override;
    void onDataReceived(const std::vector<SignalSample> &) override;
    void onDataReceived(std::vector<SignalSample> &&) override;
};
*/
#endif //SIGNAL_READER_H
