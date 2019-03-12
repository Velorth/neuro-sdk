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

#ifndef DEVICE_COMMAND_H
#define DEVICE_COMMAND_H

#include <condition_variable>
#include <vector>
#include <functional>
#include "logger.h"

namespace Neuro {

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeoutMs>
class CommandData
{
public:
	typedef CommandType cmd_type;
	typedef ErrorType error_type;

	static constexpr int MAX_LIFETIME_MS = MaxLifetimeMs;
	static constexpr int SEND_TIMEOUT_MS = SendTimeoutMs;

	CommandData(CommandType command);
	CommandData(const CommandData &) = delete;
	CommandData& operator=(const CommandData&) = delete;

	~CommandData() = default;

	CommandType getCommand() const;
	ErrorType getError() const;
	void setRequestData(const std::vector<unsigned char>);
	void wait();
	void onResponseReceived(const unsigned char *, size_t);
	void onErrorResponse(ErrorType);
	bool isResponseReceived() const { return responseReceived; }
	bool commandEquals(CommandType);
	size_t getResponseLength();
	std::vector<unsigned char> getResponseData();
	size_t getRequestLength();
	std::vector<unsigned char> getRequestData();

private:
	CommandType cmd;
	ErrorType commandError;
	std::condition_variable responseReadyCondition;
	std::mutex responseMutex;
	std::function<void()> responseCallback;
	bool responseReceived;
	std::vector<unsigned char> requestData;
	size_t requestLength;
	std::vector<unsigned char> responseData;
};

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
constexpr int CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::MAX_LIFETIME_MS;

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
constexpr int CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::SEND_TIMEOUT_MS;

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::CommandData(CommandType command) : cmd(command),
commandError(ErrorType::NO_ERROR),
responseReceived(false) {}


template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
CommandType CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::getCommand() const {

	return cmd;
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
ErrorType CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::getError() const {

	return commandError;
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
void CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::setRequestData(const std::vector<unsigned char> request_data) {

	if (request_data.size() != 0) {

		requestData = request_data;
	}

	auto log = LoggerFactory::getCurrentPlatformLogger();
	log->debug("[%s] Set request data. Command %u", __FUNCTION__, cmd);
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
void CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::onResponseReceived(const unsigned char* response_data, size_t response_length) {

	if (response_length != 0) {

		responseData.assign(response_data, response_data + response_length);
	}

	auto log = LoggerFactory::getCurrentPlatformLogger();
	log->debug("[%s] Response received. Command %d", __FUNCTION__, cmd);
	std::unique_lock<std::mutex> responseLock(responseMutex);
	responseReceived = true;
	responseReadyCondition.notify_all();
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
void CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::onErrorResponse(ErrorType error) {

	commandError = error;

	auto log = LoggerFactory::getCurrentPlatformLogger();
	log->debug("[%s] OnErrorResponse. Error %d", __FUNCTION__, commandError);

	std::unique_lock<std::mutex> responseLock(responseMutex);
	responseReceived = true;
	responseReadyCondition.notify_all();
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
bool CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::commandEquals(CommandType command) {

	return cmd == command;
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
void CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::wait() {

	std::unique_lock<std::mutex> waitLock(responseMutex);
	auto log = LoggerFactory::getCurrentPlatformLogger();
	if (!responseReceived) {

		auto timeResult = responseReadyCondition.wait_for(waitLock, std::chrono::milliseconds(MAX_LIFETIME_MS));
		if (timeResult == std::cv_status::timeout) {

			commandError = ErrorType::SEND_TIMEOUT;
			log->debug("[%s] On timeout. Command %d", __FUNCTION__, cmd);
		}
	}
	log->debug("[%s] Response ready", __FUNCTION__);
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
std::vector<unsigned char> CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::getResponseData() {

	return responseData;
}
template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
std::vector<unsigned char> CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::getRequestData() {

	return requestData;
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
size_t CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::getResponseLength() {

	return responseData.size();
}

template <typename CommandType, typename ErrorType, int MaxLifetimeMs, int SendTimeousMs>
size_t CommandData<CommandType, ErrorType, MaxLifetimeMs, SendTimeousMs>::getRequestLength() {

	return requestData.size();
}

}

#endif //DEVICE_COMMAND_H
