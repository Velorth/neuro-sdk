#ifndef COLIBRI_COMMAND_H
#define COLIBRI_COMMAND_H

#include "command_data.h"

#define COLIBRI_CMD_MAX_LIFETIME_MS 4000
#define COLIBRI_CMD_SEND_TIMEOUT_MS 2000
#define COLIBRI_CMD_SEND_MAX_ATTEMPTS 2

namespace Neuro {

enum class ColibriCommand: unsigned char
{
    ERROR = 0x00,
    ECHO = 0xA0,
    GET_ADDR = 0xA1,
    ACTIVATE_APP = 0x54,
    GET_SENSOR_PARAM = 0x24,
    SWITCH_FILTER_STATE = 0x09,
    GET_BATTERY_V = 0xA4,
    START_ADC_DATA_THROW = 0x40,
    STOP_ADC_DATA_THROW = 0x41,
    SET_FSAM = 0x03,
    SET_DATA_OFFSET = 0x21,
    SET_PGA_GAIN = 0x0D,
    START_STIM = 0x11,
    STOP_STIM = 0x12,
    SET_STIM_PARAM = 0x13,
    GET_STIM_PARAM = 0x15,
    SH_START = 0x71,
    SH_STOP = 0x72,
    SET_SH_PARAM = 0x70,
    GET_SH_PARAM = 0x73,
    GET_SH_AND_STIM_STATE = 0x17,
    DO_CALIBRATION = 0x16,
    GET_ELECTRODE_STATE = 0x46,
    SWITCH_ADC_INP = 0x0A,
    SWITCH_EXT_COM_INPUTS = 0x23
};

enum class ColibriCommandError: unsigned char
{
    NO_ERROR = 0x00,
    ERR_NO_CMD = 0x21,
    ERR_WRONG_PARAM	= 0x25,
    ERR_CS = 0x23,
    ADC_TIMEOUT_ERROR = 0x26,
    SEND_TIMEOUT = 0xFF,
    UNKNOWN_ERROR = 0xFB
};


typedef CommandData<ColibriCommand, ColibriCommandError, COLIBRI_CMD_MAX_LIFETIME_MS, COLIBRI_CMD_SEND_TIMEOUT_MS> CallibriCommandData;


enum class ColibriMode
{
    ERROR,
    BOOTLOADER,
    APPLICATION
};

inline bool parseCommand(unsigned char commandByte, ColibriCommand* outCommand)
{
    switch (commandByte)
    {
        case static_cast<unsigned char>(ColibriCommand::ERROR):
            *outCommand = ColibriCommand::ERROR;
            return true;
        case static_cast<unsigned char>(ColibriCommand::ECHO):
            *outCommand = ColibriCommand::ECHO;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_ADDR):
            *outCommand = ColibriCommand::GET_ADDR;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_BATTERY_V):
            *outCommand = ColibriCommand::GET_BATTERY_V;
            return true;
        case static_cast<unsigned char>(ColibriCommand::ACTIVATE_APP):
            *outCommand = ColibriCommand::ACTIVATE_APP;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_SENSOR_PARAM):
            *outCommand = ColibriCommand::GET_SENSOR_PARAM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SWITCH_FILTER_STATE):
            *outCommand = ColibriCommand::SWITCH_FILTER_STATE;
            return true;
        case static_cast<unsigned char>(ColibriCommand::START_ADC_DATA_THROW):
            *outCommand = ColibriCommand::START_ADC_DATA_THROW;
            return true;
        case static_cast<unsigned char>(ColibriCommand::STOP_ADC_DATA_THROW):
            *outCommand = ColibriCommand::STOP_ADC_DATA_THROW;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SET_FSAM):
            *outCommand = ColibriCommand::SET_FSAM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SET_DATA_OFFSET):
            *outCommand = ColibriCommand::SET_DATA_OFFSET;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SET_PGA_GAIN):
            *outCommand = ColibriCommand::SET_PGA_GAIN;
            return true;
        case static_cast<unsigned char>(ColibriCommand::START_STIM):
            *outCommand = ColibriCommand::START_STIM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::STOP_STIM):
            *outCommand = ColibriCommand::STOP_STIM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SET_STIM_PARAM):
            *outCommand = ColibriCommand::SET_STIM_PARAM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_STIM_PARAM):
            *outCommand = ColibriCommand::GET_STIM_PARAM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SH_START):
            *outCommand = ColibriCommand::SH_START;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SH_STOP):
            *outCommand = ColibriCommand::SH_STOP;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SET_SH_PARAM):
            *outCommand = ColibriCommand::SET_SH_PARAM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_SH_PARAM):
            *outCommand = ColibriCommand::GET_SH_PARAM;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_SH_AND_STIM_STATE):
            *outCommand = ColibriCommand::GET_SH_AND_STIM_STATE;
            return true;
        case static_cast<unsigned char>(ColibriCommand::DO_CALIBRATION):
            *outCommand = ColibriCommand::DO_CALIBRATION;
            return true;
        case static_cast<unsigned char>(ColibriCommand::GET_ELECTRODE_STATE):
            *outCommand = ColibriCommand::GET_ELECTRODE_STATE;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SWITCH_ADC_INP):
            *outCommand = ColibriCommand::SWITCH_ADC_INP;
            return true;
        case static_cast<unsigned char>(ColibriCommand::SWITCH_EXT_COM_INPUTS):
            *outCommand = ColibriCommand::SWITCH_EXT_COM_INPUTS;
            return true;
        default:
            return false;
    }
}

inline bool parseError(unsigned char errorByte, ColibriCommandError* outError)
{
    switch (errorByte)
    {
        case static_cast<unsigned char>(ColibriCommandError::NO_ERROR):
            *outError = ColibriCommandError ::NO_ERROR;
            return true;
        case static_cast<unsigned char>(ColibriCommandError::ERR_NO_CMD):
            *outError = ColibriCommandError::ERR_NO_CMD;
            return true;
        case static_cast<unsigned char>(ColibriCommandError::ERR_WRONG_PARAM):
            *outError = ColibriCommandError::ERR_WRONG_PARAM;
            return true;
        case static_cast<unsigned char>(ColibriCommandError::ERR_CS):
            *outError = ColibriCommandError::ERR_CS;
            return true;
        case static_cast<unsigned char>(ColibriCommandError::ADC_TIMEOUT_ERROR):
            *outError = ColibriCommandError::ADC_TIMEOUT_ERROR;
            return true;
        default:
            return false;
    }
}

}

#endif // COLIBRI_COMMAND_H
