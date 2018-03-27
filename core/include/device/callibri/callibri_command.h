#ifndef COLIBRI_COMMAND_H
#define COLIBRI_COMMAND_H

#include "device/command_data.h"

#define COLIBRI_CMD_MAX_LIFETIME_MS 4000
#define COLIBRI_CMD_SEND_TIMEOUT_MS 2000
#define COLIBRI_CMD_SEND_MAX_ATTEMPTS 2

namespace Neuro {

enum class CallibriCommand: unsigned char {
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

enum class CallibriError: unsigned char {
    NO_ERROR = 0x00,
    ERR_NO_CMD = 0x21,
    ERR_WRONG_PARAM	= 0x25,
    ERR_CS = 0x23,
    ADC_TIMEOUT_ERROR = 0x26,
    SEND_TIMEOUT = 0xFF,
    UNKNOWN_ERROR = 0xFB
};


typedef CommandData<CallibriCommand, CallibriError, COLIBRI_CMD_MAX_LIFETIME_MS, COLIBRI_CMD_SEND_TIMEOUT_MS> CallibriCommandData;


enum class ColibriMode {
    ERROR,
    BOOTLOADER,
    APPLICATION
};

inline bool parseCommand(unsigned char commandByte, CallibriCommand* outCommand) {
    switch (commandByte) {
        case static_cast<unsigned char>(CallibriCommand::ERROR):
            *outCommand = CallibriCommand::ERROR;
            return true;
        case static_cast<unsigned char>(CallibriCommand::ECHO):
            *outCommand = CallibriCommand::ECHO;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_ADDR):
            *outCommand = CallibriCommand::GET_ADDR;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_BATTERY_V):
            *outCommand = CallibriCommand::GET_BATTERY_V;
            return true;
        case static_cast<unsigned char>(CallibriCommand::ACTIVATE_APP):
            *outCommand = CallibriCommand::ACTIVATE_APP;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_SENSOR_PARAM):
            *outCommand = CallibriCommand::GET_SENSOR_PARAM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SWITCH_FILTER_STATE):
            *outCommand = CallibriCommand::SWITCH_FILTER_STATE;
            return true;
        case static_cast<unsigned char>(CallibriCommand::START_ADC_DATA_THROW):
            *outCommand = CallibriCommand::START_ADC_DATA_THROW;
            return true;
        case static_cast<unsigned char>(CallibriCommand::STOP_ADC_DATA_THROW):
            *outCommand = CallibriCommand::STOP_ADC_DATA_THROW;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SET_FSAM):
            *outCommand = CallibriCommand::SET_FSAM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SET_DATA_OFFSET):
            *outCommand = CallibriCommand::SET_DATA_OFFSET;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SET_PGA_GAIN):
            *outCommand = CallibriCommand::SET_PGA_GAIN;
            return true;
        case static_cast<unsigned char>(CallibriCommand::START_STIM):
            *outCommand = CallibriCommand::START_STIM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::STOP_STIM):
            *outCommand = CallibriCommand::STOP_STIM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SET_STIM_PARAM):
            *outCommand = CallibriCommand::SET_STIM_PARAM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_STIM_PARAM):
            *outCommand = CallibriCommand::GET_STIM_PARAM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SH_START):
            *outCommand = CallibriCommand::SH_START;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SH_STOP):
            *outCommand = CallibriCommand::SH_STOP;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SET_SH_PARAM):
            *outCommand = CallibriCommand::SET_SH_PARAM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_SH_PARAM):
            *outCommand = CallibriCommand::GET_SH_PARAM;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_SH_AND_STIM_STATE):
            *outCommand = CallibriCommand::GET_SH_AND_STIM_STATE;
            return true;
        case static_cast<unsigned char>(CallibriCommand::DO_CALIBRATION):
            *outCommand = CallibriCommand::DO_CALIBRATION;
            return true;
        case static_cast<unsigned char>(CallibriCommand::GET_ELECTRODE_STATE):
            *outCommand = CallibriCommand::GET_ELECTRODE_STATE;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SWITCH_ADC_INP):
            *outCommand = CallibriCommand::SWITCH_ADC_INP;
            return true;
        case static_cast<unsigned char>(CallibriCommand::SWITCH_EXT_COM_INPUTS):
            *outCommand = CallibriCommand::SWITCH_EXT_COM_INPUTS;
            return true;
        default:
            return false;
    }
}

inline bool parseError(unsigned char errorByte, CallibriError* outError) {
    switch (errorByte) {
        case static_cast<unsigned char>(CallibriError::NO_ERROR):
            *outError = CallibriError::NO_ERROR;
            return true;
        case static_cast<unsigned char>(CallibriError::ERR_NO_CMD):
            *outError = CallibriError::ERR_NO_CMD;
            return true;
        case static_cast<unsigned char>(CallibriError::ERR_WRONG_PARAM):
            *outError = CallibriError::ERR_WRONG_PARAM;
            return true;
        case static_cast<unsigned char>(CallibriError::ERR_CS):
            *outError = CallibriError::ERR_CS;
            return true;
        case static_cast<unsigned char>(CallibriError::ADC_TIMEOUT_ERROR):
            *outError = CallibriError::ADC_TIMEOUT_ERROR;
            return true;
        default:
            return false;
    }
}

}

#endif // COLIBRI_COMMAND_H
