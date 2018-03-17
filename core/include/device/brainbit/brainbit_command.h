#ifndef BRAINBIT_COMMAND_H
#define BRAINBIT_COMMAND_H

#include "device/command_data.h"

#define BRAINBIT_CMD_MAX_LIFETIME_MS 4000
#define BRAINBIT_CMD_SEND_TIMEOUT_MS 2000

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

typedef CommandData<BrainbitCommand, BrainbitCommandError, BRAINBIT_CMD_MAX_LIFETIME_MS, BRAINBIT_CMD_SEND_TIMEOUT_MS> BrainbitCommandData;

#endif // BRAINBIT_COMMAND_H
