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

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <thread>
#include "logger.h"

namespace Neuro {

template <class CommandData>
class RequestHandler
{
public:
    typedef typename CommandData::cmd_type CommandType;

    RequestHandler(std::function<void (std::shared_ptr<CommandData>)> = std::function<void(std::shared_ptr<CommandData>)>());
    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;
    ~RequestHandler();

    void setSendFunction(std::function<void(std::shared_ptr<CommandData>)>);
    void sendRequest(std::shared_ptr<CommandData>);
    void onCommandResponse(CommandType, const unsigned char*, size_t);
private:
    typedef typename CommandData::error_type ErrorType;

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
RequestHandler<CommandData>::RequestHandler(std::function<void (std::shared_ptr<CommandData>)> func)
{
    sendCommandFunc = func;
    threadStop.store(false);
    requestWorker = std::thread([=]()
                           {
                               auto log = LoggerFactory::getCurrentPlatformLogger();
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
                                       log->debug("[%s] Sending command %d. Requests left: %zd", __FUNCTION__, requestCommandData->getCommand(), requestQueue.size());
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
                                   log->debug("[%s] Pop command record", __FUNCTION__);
                                   requestQueue.pop();
                               }
                           });
}

template <class CommandData>
RequestHandler<CommandData>::~RequestHandler()
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Request handler destructor", "Request handler", __FUNCTION__);
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
    log->debug("[%s: %s] Request handler destructor EXIT", "Request handler", __FUNCTION__);
}

template<class CommandData>
void RequestHandler<CommandData>::setSendFunction(std::function<void (std::shared_ptr<CommandData>)> func){
    sendCommandFunc = func;
}

template <class CommandData>
void RequestHandler<CommandData>::sendRequest(std::shared_ptr<CommandData> requestData)
{
    std::unique_lock<std::mutex> queueLock(queueMutex);

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s] Send request. Command %d", __FUNCTION__, requestData->getCommand());
    requestQueue.emplace(
            std::pair<std::chrono::steady_clock::time_point, std::shared_ptr<CommandData>>(std::chrono::steady_clock::now(),
                                                               requestData));
    queueEmptyCondition.notify_all();
}

template <class CommandData>
void RequestHandler<CommandData>::onCommandResponse(CommandType cmd, const unsigned char* data, size_t data_length)
{
    std::unique_lock<std::mutex> requestLock(requestMutex);
    std::unique_lock<std::mutex> queueChangeLock(queueMutex);

        auto log = LoggerFactory::getCurrentPlatformLogger();

    if (requestQueue.size() == 0)
    {
        log->warn("[%s] Response queue is empty. Command %d", __FUNCTION__, cmd);
        return;
    }



    log->debug("[%s] Response received. Command %d", __FUNCTION__, cmd);
    auto requestData = requestQueue.front().second;
    if (requestData->commandEquals(cmd))
    {
        log->debug("[%s] Command equals. Command %d", __FUNCTION__, cmd);
        requestData->onResponseReceived(data, data_length);
        responseReceivedCondition.notify_all();
    }
    else if (cmd == CommandType::ERROR)
    {
        log->debug("[%s] Command error. Command %d", __FUNCTION__, cmd);

        if (data_length > 0) {
            ErrorType error;
            if (!parseError(data[0], &error)) {
                log->error("[%s] Unable to parse error with code %d", __FUNCTION__, data[0]);
            }
            requestData->onErrorResponse(error);
        }
        else{
            log->error("[%s] Unable to parse error: no error code received", __FUNCTION__);
        }

        responseReceivedCondition.notify_all();
    }
}

}

#endif //REQUESTHANDLER_H
