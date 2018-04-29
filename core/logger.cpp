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

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include "logger.h"


Logger::Logger():
    currentLogLevel(DefaultLogLevel)
{

}

Logger::~Logger() {

}


void Logger::trace(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Trace, format, fmtList);
    va_end(fmtList);
}

void Logger::debug(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Debug, format, fmtList);
    va_end(fmtList);
}

void Logger::info(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Info, format, fmtList);
    va_end(fmtList);
}

void Logger::warn(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Warn, format, fmtList);
    va_end(fmtList);
}

void Logger::error(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Error, format, fmtList);
    va_end(fmtList);
}

void Logger::printLogMessage(LogLevel logLevel, const char *format, va_list args)
{
    if (currentLogLevel > logLevel) return;

    std::unique_lock<std::mutex> writeLogLock(writeLogMutex);

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[128];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,128,"%d-%m-%Y %I:%M:%S",timeinfo);
    fprintf(stderr, "%s", buffer);
    fprintf(stderr, "|");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}


LoggerFactory* LoggerFactory::factoryInstance = nullptr;

LoggerFactory::LoggerFactory():
    currentLogger(new Logger())
{

}

LoggerFactory::~LoggerFactory()
{
    delete currentLogger;
}

LoggerFactory *LoggerFactory::getInstance()
{
    if (factoryInstance == nullptr)
    {
        factoryInstance = new LoggerFactory();
    }

    return factoryInstance;
}

Logger *LoggerFactory::getCurrentPlatformLogger()
{
    auto instance = getInstance();
    return instance->currentLogger;
}

void LoggerFactory::setLogger(Logger *logger)
{
    delete currentLogger;
    currentLogger = logger;
}

void LoggerFactory::setLogLevel(LogLevel level)
{
    auto instance = getInstance();
    instance->currentLogger->setLogLevel(level);
}





