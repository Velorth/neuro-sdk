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

#ifndef REQUEST_SCHEDULER_H
#define REQUEST_SCHEDULER_H

#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>
#include "logger.h"

namespace Neuro {

template <class CommandData>
class RequestScheduler
{
public:
    typedef typename CommandData::cmd_type CommandType;

    RequestScheduler(std::function<void (std::shared_ptr<CommandData>)> = std::function<void(std::shared_ptr<CommandData>)>());
    RequestScheduler(const RequestScheduler&) = delete;
    RequestScheduler& operator=(const RequestScheduler&) = delete;
    ~RequestScheduler();

    void setSendFunction(std::function<void(std::shared_ptr<CommandData>)>);
    void sendRequest(std::shared_ptr<CommandData>);
    void onCommandResponse(CommandType, const unsigned char*, size_t);
private:
    typedef typename CommandData::error_type ErrorType;

    static constexpr const char *class_name = "RequestScheduler";

    std::queue<std::pair<std::chrono::steady_clock::time_point, std::shared_ptr<CommandData>>> requestQueue;
    std::atomic<bool> threadStop;
    std::mutex queueMutex;
    std::mutex requestMutex;
    std::condition_variable queueEmptyCondition;
    std::condition_variable responseReceivedCondition;
    std::thread requestWorker;
    std::function<void(std::shared_ptr<CommandData>)> sendCommandFunc;
};

template <class CommandData>
RequestScheduler<CommandData>::RequestScheduler(std::function<void (std::shared_ptr<CommandData>)> func)
{
    sendCommandFunc = func;
    threadStop.store(false);
    requestWorker = std::thread([=]()
                           {
                               while (!threadStop.load())
                               {
                                   std::unique_lock<std::mutex> queueCheckLock(queueMutex);
                                   if (requestQueue.size() == 0)
                                   {
                                       queueEmptyCondition.wait(queueCheckLock);
                                       continue;
                                   }
                                   queueCheckLock.unlock();

                                   //calculating deadline for next request
                                   auto requestPair = requestQueue.front();
                                   auto pushTime = requestPair.first;
                                   auto requestCommandData = requestPair.second;
                                   auto deadline =
                                           pushTime + std::chrono::milliseconds(CommandData::MAX_LIFETIME_MS);

                                   while (std::chrono::steady_clock::now() < deadline)
                                   {
                                       std::unique_lock<std::mutex> requestLock(requestMutex);

                                       //we should check if response been received while mutex been
                                       //in released state here, but was acquired in responseReceived method
                                       if (requestCommandData->isResponseReceived()) break;

                                       //trying to send request to a beacon until deadline reached
                                       LOG_TRACE_V("Sending command %d. Requests left: %zd", requestCommandData->getCommand(), requestQueue.size());
                                       if (sendCommandFunc) sendCommandFunc(requestCommandData);
                                       auto status = responseReceivedCondition.wait_for(requestLock,
                                                                                        std::chrono::milliseconds(
                                                                                                CommandData::SEND_TIMEOUT_MS));

                                       requestLock.unlock();

                                       //check if thread must be stopped
                                       if (threadStop.load()) return;

                                       //if request send not timeouted than we've received response
                                       //breaking
                                       if (status == std::cv_status::no_timeout)
                                       {
                                           break;
                                       }
                                   }

                                   std::unique_lock<std::mutex> queueChangeLock(queueMutex);
                                   LOG_TRACE("Pop command record");
                                   requestQueue.pop();
                               }
                           });
}

template <class CommandData>
RequestScheduler<CommandData>::~RequestScheduler()
{
    LOG_TRACE("Request handler destructor");
    threadStop.store(true);
    queueEmptyCondition.notify_all();
    responseReceivedCondition.notify_all();
    try
    {
        if (requestWorker.joinable())
            requestWorker.join();
    }
    catch(std::system_error&)
    {}
    LOG_TRACE("Request handler destructor EXIT");
}

template<class CommandData>
void RequestScheduler<CommandData>::setSendFunction(std::function<void (std::shared_ptr<CommandData>)> func){
    sendCommandFunc = func;
}

template <class CommandData>
void RequestScheduler<CommandData>::sendRequest(std::shared_ptr<CommandData> requestData)
{
    std::unique_lock<std::mutex> queueLock(queueMutex);
    LOG_TRACE_V("Send request. Command %d", requestData->getCommand());
    requestQueue.emplace(
            std::pair<std::chrono::steady_clock::time_point, std::shared_ptr<CommandData>>(std::chrono::steady_clock::now(),
                                                               requestData));
    queueEmptyCondition.notify_all();
}

template <class CommandData>
void RequestScheduler<CommandData>::onCommandResponse(CommandType cmd, const unsigned char* data, size_t data_length)
{
    std::unique_lock<std::mutex> requestLock(requestMutex);
    std::unique_lock<std::mutex> queueChangeLock(queueMutex);

    if (requestQueue.size() == 0)
    {
        LOG_WARN_V("Response queue is empty. Command %d", cmd);
        return;
    }

    LOG_TRACE_V("Response received. Command %d", cmd);
    auto requestData = requestQueue.front().second;
    if (requestData->commandEquals(cmd))
    {
        LOG_TRACE_V("Command equals. Command %d", cmd);
        requestData->onResponseReceived(data, data_length);
        responseReceivedCondition.notify_all();
    }
    else if (cmd == CommandType::ERROR)
    {
        LOG_TRACE_V("Command error. Command %d", cmd);

        if (data_length > 0) {
            ErrorType error;
            if (!parseError(data[0], &error)) {
                LOG_ERROR_V("Unable to parse error with code %d", data[0]);
            }
            requestData->onErrorResponse(error);
        }
        else{
            LOG_ERROR("Unable to parse error: no error code received");
        }

        responseReceivedCondition.notify_all();
    }
}

}

#endif //REQUEST_SCHEDULER_H
