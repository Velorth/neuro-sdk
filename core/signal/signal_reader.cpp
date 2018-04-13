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

#include "signal/signal_reader.h"
#include "logger.h"

/*SignalReader::SignalReader(std::unique_ptr<CircularBuffer> signal_buffer):
        signalBuffer(std::move(signal_buffer)),
        changedLength(0),
        threadStop(false),
        lengthChangedNotificationThread(&SimpleReader::lengthChangedNotificationFunc, this)
{}


SignalReader::~SignalReader() {

    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Signal reader destructor", "SignalReader", __FUNCTION__);
    threadStop.store(true);
    std::unique_lock<std::mutex> lengthLock(notificationThreadMutex);
    lengthChangedCondition.notify_all();
    lengthLock.unlock();
    try {
        if (lengthChangedNotificationThread.joinable()) {
            lengthChangedNotificationThread.join();
        }
    }
    catch(std::system_error&)
    {}
    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Signal reader destructor EXIT", "SignalReader", __FUNCTION__);
}

size_t SignalReader::getOverallLength() const{
    return signalBuffer->getTotalAppendedDataLength();
}

void SignalReader::setChangedLength(std::size_t bufferLength){

    std::unique_lock<std::mutex> lengthLock(notificationThreadMutex);
    changedLength = bufferLength;
    lengthChangedCondition.notify_all();
}

void SignalReader::setSignalBuffer(std::unique_ptr<CircularBuffer>buffer) {
    std::unique_lock<std::mutex> bufferLock(dataBufferMutex);
    signalBuffer.reset(buffer.release());
}

void SignalReader::subscribeSignalLengthChanged(std::function<void(size_t)> callback) const {
    //TODO Support list of callbacks to notify
    signalLengthChangedCallback = callback;
}

size_t SignalReader::getBufferSize() const {
    return signalBuffer->getBufferSize();
}

void SignalReader::lengthChangedNotificationFunc(){

    std::unique_lock<std::mutex> lengthInitializationLock(notificationThreadMutex);
    auto lastLength = changedLength;
    lengthInitializationLock.unlock();

    while(!threadStop.load()){
        std::unique_lock<std::mutex> lengthChangedLock(notificationThreadMutex);
        if (changedLength == lastLength){
            lengthChangedCondition.wait(lengthChangedLock);
        }
        if (changedLength == lastLength){
            //in case of lengthChangedCondition notified from destructor
            //or new length is equal to old
            continue;
        }
        lastLength = changedLength;
        lengthChangedLock.unlock();

        if (signalLengthChangedCallback){

            signalLengthChangedCallback(lastLength);
        }
    }
}

void SignalReader::setSignalInverse(bool isReverse) {
    signalReverseNeeded.store(isReverse);
}


*/










