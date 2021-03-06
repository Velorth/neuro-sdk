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

#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include "lib_export.h"

enum class LogLevel: int
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4
};

#ifdef NDEBUG
constexpr LogLevel DefaultLogLevel = LogLevel::Info;
#else
constexpr LogLevel DefaultLogLevel = LogLevel::Debug;
#endif

class SDK_SHARED Logger
{
public:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    virtual ~Logger();

    virtual void trace(const char *format, ...);
    virtual void debug(const char *format, ...);
    virtual void info(const char *format, ...);
    virtual void warn(const char *format, ...);
    virtual void error(const char *format, ...);

    void setLogLevel(LogLevel);
protected:
    LogLevel currentLogLevel;
private:
    void printLogMessage(LogLevel logLevel, const char* format, va_list args);
    std::mutex writeLogMutex;
};

class SDK_SHARED LoggerFactory
{
public:
    LoggerFactory(const LoggerFactory&) = delete;
    LoggerFactory& operator=(const LoggerFactory&) = delete;
    ~LoggerFactory();

    static LoggerFactory *getInstance();
    static Logger* getCurrentPlatformLogger();
    static void setLogLevel(LogLevel);
    void setLogger(Logger*);

private:
    LoggerFactory();
    static LoggerFactory* factoryInstance;
    Logger* currentLogger;
};

#define LOG_ERROR(MESSAGE) LoggerFactory::getCurrentPlatformLogger()->error("[%s: %s] " MESSAGE, class_name, __FUNCTION__);
#define LOG_WARN(MESSAGE) LoggerFactory::getCurrentPlatformLogger()->warn("[%s: %s] " MESSAGE, class_name, __FUNCTION__);
#define LOG_INFO(MESSAGE) LoggerFactory::getCurrentPlatformLogger()->info("[%s: %s] " MESSAGE, class_name, __FUNCTION__);
#define LOG_DEBUG(MESSAGE) LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] " MESSAGE, class_name, __FUNCTION__);
#define LOG_TRACE(MESSAGE) LoggerFactory::getCurrentPlatformLogger()->trace("[%s: %s] " MESSAGE, class_name, __FUNCTION__);

#define LOG_ERROR_V(MESSAGE, ...) LoggerFactory::getCurrentPlatformLogger()->error("[%s: %s] " MESSAGE, class_name, __FUNCTION__, __VA_ARGS__);
#define LOG_WARN_V(MESSAGE, ...) LoggerFactory::getCurrentPlatformLogger()->warn("[%s: %s] " MESSAGE, class_name, __FUNCTION__, __VA_ARGS__);
#define LOG_INFO_V(MESSAGE, ...) LoggerFactory::getCurrentPlatformLogger()->info("[%s: %s] " MESSAGE, class_name, __FUNCTION__, __VA_ARGS__);
#define LOG_DEBUG_V(MESSAGE, ...) LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] " MESSAGE, class_name, __FUNCTION__, __VA_ARGS__);
#define LOG_TRACE_V(MESSAGE, ...) LoggerFactory::getCurrentPlatformLogger()->trace("[%s: %s] " MESSAGE, class_name, __FUNCTION__, __VA_ARGS__);

#endif // LOGGER_H

