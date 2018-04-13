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

#include "signal/channel.h"
#include "logger.h"
#include "saturation_cast.h"
#include <thread>
#include <functional>
#include <algorithm.h>
/*
using std::thread;
using std::string;
using std::vector;

Channel::Channel(int number, string name, SignalReader &reader):
        channelName(name),
        channelNumber(number),
        signalReader(reader),
        changedLength(0),
        threadStop(false),
        lengthChangedNotificationThread(&Channel::lengthChangedNotificationFunc, this) {

    signalReader.subscribeSignalLengthChanged(
            [this](size_t signalLength) { onSignalLengthChanged(signalLength); });
}


Channel::~Channel() {

    threadStop.store(true);
    std::unique_lock<std::mutex> lengthChangedLock(notificationThreadMutex);
    lengthChangedCondition.notify_all();
    lengthChangedLock.unlock();
    lengthChangedNotificationThread.join();
}

string Channel::getName() const {

    return channelName;
}

size_t Channel::getDataLength() {

    return signalReader.getOverallLength();
}

std::vector<SignalSample> Channel::getSignalData(int offset, size_t length) {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Read length: %d", "Channel", __FUNCTION__, length);

    vector<SignalSample> outVector(length);
    if (offset < 0)
    {
        log->error("[%s] Cant read data for channel. Offset is invalid", __FUNCTION__);
        return outVector;
    }

    if (filteredDataReader) {
       log->trace("[%s] Returning filtered data", __FUNCTION__);
       return filteredDataReader->readDataForChannel(channelNumber, saturation_cast<size_t>(offset), length);
    }
    else
    {
        log->trace("[%s] Returning raw data", __FUNCTION__);
        return signalReader.readDataForChannel(channelNumber, saturation_cast<size_t>(offset), length);
    }
}


std::vector<SignalSample> Channel::getRawData(int offset, size_t length) {

    return signalReader.readDataForChannel(channelNumber, saturation_cast<size_t>(offset), length);
}

bool Channel::addFilter(std::unique_ptr<SignalFilter> filter, bool isReverse) {

    if (!filteredDataReader){

        std::unique_ptr<CircularBuffer> filteredDataBuffer(new CircularBuffer(signalReader.getBufferSize()));
        filteredDataReader.reset(new SimpleReader(std::move(filteredDataBuffer)));
    }

    channelFilters.push_back(ChannelFilter{std::move(filter), isReverse});
    return true;
}

void Channel::clearFilters() {
    channelFilters.clear();
}

void Channel::onSignalLengthChanged(size_t newLength) {
    if (filteredDataReader) {
        auto rawSignalLength = signalReader.getOverallLength();

        auto lengthDifference = rawSignalLength - filteredDataReader->getOverallLength();
        auto log = LoggerFactory::getCurrentPlatformLogger();

        if (lengthDifference == 0) {
            log->debug("[%s] Nothing to filter", __FUNCTION__);
            return;
        }

        auto rawOffset = filteredDataReader->getOverallLength();
        auto rawData = signalReader.readDataForChannel(channelNumber, rawOffset, lengthDifference);
        for (auto &sample : rawData) {
            for (auto &signalFilter : channelFilters) {
                auto isReverse = signalFilter.isReverse;
                if (!isReverse) {
                    sample = signalFilter.filter->filter(sample);
                } else {
                    sample -= signalFilter.filter->filter(sample);
                }
            }
        }
        filteredDataReader->onDataReceived(rawData);
        auto bufferLength = filteredDataReader->getOverallLength();
        if (bufferLength!=newLength){
            log->warn("[%s: %s] Filtered data length is not equal to raw data length", "Channel",__FUNCTION__);
        }
    }

    std::unique_lock<std::mutex> lengthLock(notificationThreadMutex);
    changedLength = newLength;
    lengthChangedCondition.notify_all();
}


void Channel::lengthChangedNotificationFunc(){

    std::unique_lock<std::mutex> lengthInitializationLock(notificationThreadMutex);
    auto lastLength = changedLength;
    lengthInitializationLock.unlock();

    //auto log = LoggerFactory::getCurrentPlatformLogger();
    //auto count = 0;
    //auto interval = 0;
    while(!threadStop.load()){
        std::unique_lock<std::mutex> lengthChangedLock(notificationThreadMutex);
        if (changedLength == lastLength){
            lengthChangedCondition.wait(lengthChangedLock);
        }
        if (changedLength == lastLength){
            continue;
        }
        lastLength = changedLength;
        lengthChangedLock.unlock();

        signalLengthChanged(*this, lastLength);
    }
}

SpectrumData<SignalSample> Channel::calculateSpectrum(std::size_t offset, std::size_t length, int samplingFrequency) {

    auto data = getSignalData(saturation_cast<int>(offset), length);
    auto spectrumSize = alg::next_power_2(data.size());
    data.resize(spectrumSize);

    auto begin = std::chrono::high_resolution_clock::now();

    auto spectrum = alg::fft(data);

    auto end = std::chrono::high_resolution_clock::now();
    auto interval = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] FFT takes %f ms", "Channel", __FUNCTION__, interval);

    auto frequencyStep = static_cast<double>(samplingFrequency)/spectrumSize;

    return SpectrumData<SignalSample>(spectrum, frequencyStep);
}

void Channel::setSignalInverse(bool isInverse) {
    signalReader.setSignalInverse(isInverse);
}



*/




























