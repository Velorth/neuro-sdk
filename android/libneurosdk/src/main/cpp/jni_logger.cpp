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

#include "jni_logger.h"
#include "android/log.h"

JniLogger::~JniLogger() {

}

void JniLogger::trace(const char *format, ...) {
    if (currentLogLevel > LogLevel::Trace) return;

    va_list formatData;
    va_start(formatData, format);
    __android_log_vprint(ANDROID_LOG_VERBOSE, NEURO_CORE_LOG_TAG, format, formatData);
    va_end(formatData);
}

void JniLogger::info(const char *format, ...) {
    if (currentLogLevel > LogLevel::Info) return;

    va_list formatData;
    va_start(formatData, format);
    __android_log_vprint(ANDROID_LOG_INFO, NEURO_CORE_LOG_TAG, format, formatData);
    va_end(formatData);
}

void JniLogger::debug(const char *format, ...) {
    if (currentLogLevel > LogLevel::Debug) return;

    va_list formatData;
    va_start(formatData, format);
    __android_log_vprint(ANDROID_LOG_DEBUG, NEURO_CORE_LOG_TAG, format, formatData);
    va_end(formatData);
}

void JniLogger::warn(const char *format, ...) {
    if (currentLogLevel > LogLevel::Warn) return;

    va_list formatData;
    va_start(formatData, format);
    __android_log_vprint(ANDROID_LOG_WARN, NEURO_CORE_LOG_TAG, format, formatData);
    va_end(formatData);
}

void JniLogger::error(const char *format, ...) {
    if (currentLogLevel > LogLevel::Error) return;

    va_list formatData;
    va_start(formatData, format);
    __android_log_vprint(ANDROID_LOG_ERROR, NEURO_CORE_LOG_TAG, format, formatData);
    va_end(formatData);
}














