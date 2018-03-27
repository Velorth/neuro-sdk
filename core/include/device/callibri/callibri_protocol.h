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

#ifndef CALLIBRI_PROTOCOL_H
#define CALLIBRI_PROTOCOL_H

#include "common_types.h"
#include "device/device_parameters.h"

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

namespace Neuro {

using callibri_marker_t = unsigned short;

namespace {
constexpr callibri_marker_t CallibriCommandMarker = 65503;
constexpr callibri_marker_t CallibriMemsMarker = 65501;
constexpr callibri_marker_t CallibriRespirationMarker = 65502;
constexpr callibri_marker_t CallibriAngleMarker = 65505;
}

constexpr callibri_marker_t CallibriMaxPacketNumber = 65500;
constexpr std::size_t CallibriPacketSize = 20;
constexpr std::size_t CallibriMarkerPosition = 0;
constexpr std::size_t CallibriMarkerLength = 2;

enum class CallibriPacketType {
    Command,
    Signal,
    MEMS,
    Respiration,
    Angle
};

inline CallibriPacketType fromMarker(callibri_marker_t marker){
    if (marker <= CallibriMaxPacketNumber){
        return CallibriPacketType::Signal;
    }
    switch (marker){
    case CallibriCommandMarker:
        return CallibriPacketType::Command;
    case CallibriMemsMarker:
        return CallibriPacketType::MEMS;
    case CallibriAngleMarker:
        return CallibriPacketType::Angle;
    case CallibriRespirationMarker:
        return CallibriPacketType::Respiration;
    default:
        throw std::runtime_error("Unresolved marker value");
    }
}

template <typename T>
Byte byteCode(T value);

template<>
inline Byte byteCode<Gain>(Gain value){
    switch (value){
    case Gain::Gain1:
        return Byte{0x01};
    case Gain::Gain2:
        return Byte{0x02};
    case Gain::Gain3:
        return Byte{0x03};
    case Gain::Gain4:
        return Byte{0x04};
    case Gain::Gain6:
        return Byte{0x00};//(sic!)
    case Gain::Gain8:
        return Byte{0x05};
    case Gain::Gain12:
        return Byte{0x06};
    default:
        throw std::runtime_error("Unsupported gain value");
    }
}

template<>
inline Byte byteCode<SamplingFrequency>(SamplingFrequency value){
    switch (value)
    {
    case SamplingFrequency::Hz125:
        return Byte{0x00};
    case SamplingFrequency::Hz250:
        return Byte{0x01};;
    case SamplingFrequency::Hz500:
        return Byte{0x02};
    case SamplingFrequency::Hz1000:
        return Byte{0x03};
    case SamplingFrequency::Hz2000:
        return Byte{0x04};
    case SamplingFrequency::Hz4000:
        return Byte{0x05};
    case SamplingFrequency::Hz8000:
        return Byte{0x06};
    default:
        throw std::runtime_error("Unsupported sampling frequency value");
    }
}

template<>
inline Byte byteCode<ADCInput>(ADCInput value){
    switch (value)
    {
    case ADCInput::Electrodes:
        return Byte{0x00};
    case ADCInput::Short:
        return Byte{0x01};;
    case ADCInput::Test:
        return Byte{0x02};
    case ADCInput::Resistance:
        return Byte{0x03};
    default:
        throw std::runtime_error("Unsupported ADC input value");
    }
}

template<>
inline Byte byteCode<ExternalSwitchInput>(ExternalSwitchInput value){
    switch (value)
    {
    case ExternalSwitchInput::MioElectrodesRespUSB:
        return Byte{0x00};
    case ExternalSwitchInput::MioElectrodes:
        return Byte{0x01};;
    case ExternalSwitchInput::MioUSB:
        return Byte{0x02};
    case ExternalSwitchInput::RespUSB:
        return Byte{0x03};
    default:
        throw std::runtime_error("Unsupported external switch input value");
    }
}

}
#endif //COLIBRI_PROTOCOL_H
