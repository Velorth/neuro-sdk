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

#include "logger_objc.h"
#import "Foundation/Foundation.h"

ObjcLogger::~ObjcLogger(){
    
}

void ObjcLogger::trace(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Trace, format, fmtList);
    va_end(fmtList);
}

void ObjcLogger::debug(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Debug, format, fmtList);
    va_end(fmtList);
}

void ObjcLogger::info(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Info, format, fmtList);
    va_end(fmtList);
}

void ObjcLogger::warn(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Warn, format, fmtList);
    va_end(fmtList);
}

void ObjcLogger::error(const char *format, ...)
{
    va_list fmtList;
    va_start(fmtList, format);
    printLogMessage(LogLevel::Error, format, fmtList);
    va_end(fmtList);
}

void ObjcLogger::printLogMessage(LogLevel logLevel, const char *format, va_list args)
{
    if (currentLogLevel > logLevel) return;
    
    std::unique_lock<std::mutex> writeLogLock(writeLogMutex);
    
    NSLogv([NSString stringWithCString:format encoding:NSASCIIStringEncoding], args);
}
