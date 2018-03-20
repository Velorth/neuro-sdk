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

#ifndef COLIBRI_PROTOCOL_H
#define COLIBRI_PROTOCOL_H

#include <initializer_list>

#define COLIBRI_SORT_MAP_SIZE 250
#define COLIBRI_BUFFER_DURATION 60

#define COLIBRI_PACKET_SIZE 20u

#define COLIBRI_HEADER_CMD_POSITION 3
#define COLIBRI_HEADER_ADDR_POSITION 4
#define COLIBRI_HEADER_CS_POSITION 7
#define COLIBRI_CMD_HDR_DATA_START_POS 8
#define COLIBRI_SIGNAL_DATA_START_POS 2
#define COLIBRI_MEMS_DATA_START_POS 4
#define COLIBRI_SPIRO_DATA_START_POS 4

#define COLIBRI_ADC_CAPACITY 8388607
#define COLIBRI_ADC_REF_VOLTAGE 2.42

#define COLIBRI_COMMAND_MARKER 0xFFDF
#define COLIBRI_MEMS_DATA_MARKER 0xFFDD
#define COLIBRI_SPIRO_DATA_MARKER 0xFFDE
#define COLIBRI_DATA_MAX_PACKET_NMB 0xFFDC

//changing address length to bigger values may cause
//errors with its representation as integral type values
//see usages of this macro
#define COLIBRI_ADDRESS_LENGTH 3
#define COLIBRI_HOST_ADDRESS 0xA5B6C7L

#define COLIBRI_SENSOR_PARAMS_DATA_LENGTH 8
#define COLIBRI_STIM_PARAMS_DATA_LENGTH 6
#define COLIBRI_STIM_STATE_PARAMS_DATA_LENGTH 2
#define COLIBRI_SH_PARAMS_DATA_LENGTH 5
#define COLIBRI_ADC_INPUT_MODE_BYTE_POS 2
#define COLIBRI_FILTER_STATE_BYTE_POS 3
#define COLIBRI_GAIN_BYTE_POS 4
#define COLIBRI_DATA_OFFSET_BYTE_POS 5
#define COLIBRI_EXT_SWITCH_STATE_BYTE_POS 6

#define COLIBRI_STIMULATOR_STATE_ACTIVE 2
#define COLIBRI_STIMULATOR_STATE_STOPED 1
#define COLIBRI_STIMULATOR_STATE_UNDEFINED 0

#define COLIBRI_SH_STATE_ACTIVE 2
#define COLIBRI_SH_STATE_STOPED 1
#define COLIBRI_SH_STATE_UNDEFINED 0

enum class ColibriSamplingFreq: unsigned char
{
    DATA_RATE_125SPS = 0x00,
    DATA_RATE_250SPS = 0x01,
    DATA_RATE_500SPS = 0x02,
    DATA_RATE_1kSPS = 0x03,
    DATA_RATE_2kSPS = 0x04,
    DATA_RATE_4kSPS = 0x05,
    DATA_RATE_8kSPS = 0x06,
    UNSUPPORTED = 0xFF
};

enum class ColibriGain: unsigned char{

    GAIN_1 = 0x01,
    GAIN_2 = 0x02,
    GAIN_3 = 0x03,
    GAIN_4 = 0x04,
    GAIN_6 = 0x00,
    GAIN_8 = 0x05,
    GAIN_12 = 0x06,
    UNSUPPORTED = 0xFF
};

enum class ColibriAdcInputMode: unsigned char{
    ELECTRODES = 0,
    SHORT = 1,
    ADC_TEST = 2,
    RESIST = 3
};

enum class ColibriExtSwitchState: unsigned char{
    RESPUSB_MIOELECTRODES = 0,
    RESPNONE_MIOELECTRODES = 1,
    RESPNONE_MIOUSB = 2,
    RESPUSB_MIONONE = 3
};

inline int colibriSamplingFreqToInt(ColibriSamplingFreq samplingFreq)
{
    switch (samplingFreq)
    {
        case ColibriSamplingFreq::DATA_RATE_125SPS:
            return 125;
        case ColibriSamplingFreq::DATA_RATE_250SPS:
            return 250;
        case ColibriSamplingFreq::DATA_RATE_500SPS:
            return 500;
        case ColibriSamplingFreq::DATA_RATE_1kSPS:
            return 1000;
        case ColibriSamplingFreq::DATA_RATE_2kSPS:
            return 2000;
        case ColibriSamplingFreq::DATA_RATE_4kSPS:
            return 4000;
        case ColibriSamplingFreq::DATA_RATE_8kSPS:
            return 8000;
        default:
            return 0;
    }
}

inline ColibriSamplingFreq colibriSamplingFrequencyFromInt(int frequency)
{
    switch (frequency)
    {
        case 125:
            return ColibriSamplingFreq::DATA_RATE_125SPS;
        case 250:
            return ColibriSamplingFreq::DATA_RATE_250SPS;
        case 500:
            return ColibriSamplingFreq::DATA_RATE_500SPS;
        case 1000:
            return ColibriSamplingFreq::DATA_RATE_1kSPS;
        case 2000:
            return ColibriSamplingFreq::DATA_RATE_2kSPS;
        case 4000:
            return ColibriSamplingFreq::DATA_RATE_4kSPS;
        case 8000:
            return ColibriSamplingFreq::DATA_RATE_8kSPS;
        default:
            return ColibriSamplingFreq::UNSUPPORTED;
    }
}

inline int colibriGainToInt(ColibriGain gain)
{
    switch (gain)
    {
        case ColibriGain::GAIN_1:
            return 1;
        case ColibriGain::GAIN_2:
            return 2;
        case ColibriGain::GAIN_3:
            return 3;
        case ColibriGain::GAIN_4:
            return 4;
        case ColibriGain::GAIN_6:
            return 6;
        case ColibriGain::GAIN_8:
            return 8;
        case ColibriGain::GAIN_12:
            return 12;
        default:
            return 0;
    }
}

inline ColibriGain colibriGainFromInt(int gain)
{
    switch (gain)
    {
        case 1:
            return ColibriGain::GAIN_1;
        case 2:
            return ColibriGain::GAIN_2;
        case 3:
            return ColibriGain::GAIN_3;
        case 4:
            return ColibriGain::GAIN_4;
        case 6:
            return ColibriGain::GAIN_6;
        case 8:
            return ColibriGain::GAIN_8;
        case 12:
            return ColibriGain::GAIN_12;
        default:
            return ColibriGain::UNSUPPORTED;
    }
}

inline std::initializer_list<int> colibriSupportedFrequencies()
{
    return {125, 250, 500, 1000, 2000, 4000, 8000};
}

typedef union
{
    unsigned short value;
    unsigned char bytes[sizeof(unsigned short)];
}ColibriUShort;

typedef union
{
    short value;
    unsigned char bytes[sizeof(short)];
}ColibriShort;

typedef union
{
    long value;
    unsigned char bytes[sizeof(long)];
}ColibriLong;

#endif //COLIBRI_PROTOCOL_H
