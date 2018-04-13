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

#ifndef ANDROID_JNI_LOGGER_H
#define ANDROID_JNI_LOGGER_H

#include "logger.h"

#define NEURO_CORE_LOG_TAG "NeuroCore"

class JniLogger : public Logger {
public:
    ~JniLogger();

    void trace(const char *format, ...);

    void debug(const char *format, ...);

    void info(const char *format, ...);

    void warn(const char *format, ...);

    void error(const char *format, ...);
};

#endif //ANDROID_JNI_LOGGER_H
