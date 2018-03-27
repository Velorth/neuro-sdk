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

#ifndef BRAINBIT_PROTOCOL_H
#define BRAINBIT_PROTOCOL_H

#include "device/command_data.h"

/**
 * BrainBit configuration values and constants
 */
#define BRAINBIT_PACKET_SIZE 20
#define BRAINBIT_MAX_PACKET_DELAY 10000
#define BRAINBIT_SORT_MAP_SIZE 50 //matches 200 ms delay in output buffers
#define BRAINBIT_T3_CHANNEL_INDEX 0
#define BRAINBIT_O1_CHANNEL_INDEX 1
#define BRAINBIT_T4_CHANNEL_INDEX 2
#define BRAINBIT_O2_CHANNEL_INDEX 3

#define BRAINBIT_CMD_MAX_LIFETIME_MS 4000
#define BRAINBIT_CMD_SEND_TIMEOUT_MS 2000

namespace Neuro {

enum class BrainbitCommand: unsigned char
{
    ERROR = 0x00,
    CMD_STOP = 0x01,
    CMD_SIGNAL = 0x02,
    CMD_RESIST = 0x03,
    CMD_BOOTLOADER_NEED = 0x04
};

enum class BrainbitCommandError: unsigned char
{
    NO_ERROR = 0x00,
    ERROR_LEN = 0x01,
    ERROR_MODE = 0x02,
    SEND_TIMEOUT = 0xFF,
    UNKNOWN_ERROR = 0xFB
};

inline bool parseCommand(unsigned char commandByte, BrainbitCommand* outCommand)
{
    switch (commandByte)
    {
        case static_cast<unsigned char>(BrainbitCommand::ERROR):
            *outCommand = BrainbitCommand::ERROR;
            return true;
        case static_cast<unsigned char>(BrainbitCommand::CMD_STOP):
            *outCommand = BrainbitCommand::CMD_STOP;
            return true;
        case static_cast<unsigned char>(BrainbitCommand::CMD_SIGNAL):
            *outCommand = BrainbitCommand::CMD_SIGNAL;
            return true;
        case static_cast<unsigned char>(BrainbitCommand::CMD_RESIST):
            *outCommand = BrainbitCommand::CMD_RESIST;
            return true;
        case static_cast<unsigned char>(BrainbitCommand::CMD_BOOTLOADER_NEED):
            *outCommand = BrainbitCommand::CMD_BOOTLOADER_NEED;
            return true;
        default:
            return false;
    }
}

inline bool parseError(unsigned char errorByte, BrainbitCommandError* outError)
{
    switch (errorByte)
    {
        case static_cast<unsigned char>(BrainbitCommandError::NO_ERROR):
            *outError = BrainbitCommandError::NO_ERROR;
            return true;
        case static_cast<unsigned char>(BrainbitCommandError::ERROR_LEN):
            *outError = BrainbitCommandError::ERROR_LEN;
            return true;
        case static_cast<unsigned char>(BrainbitCommandError::ERROR_MODE):
            *outError = BrainbitCommandError::ERROR_MODE;
            return true;
        default:
            return false;
    }
}

using  BrainbitCommandData = CommandData<BrainbitCommand, BrainbitCommandError, BRAINBIT_CMD_MAX_LIFETIME_MS, BRAINBIT_CMD_SEND_TIMEOUT_MS>;

}
#endif
