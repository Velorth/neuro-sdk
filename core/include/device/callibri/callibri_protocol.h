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

#ifndef CALLIBRI_PROTOCOL_H
#define CALLIBRI_PROTOCOL_H

#include "common_types.h"
#include "callibri_command.h"
#include "device/device_parameters.h"

#define COLIBRI_STIM_PARAMS_DATA_LENGTH 6
#define COLIBRI_STIM_STATE_PARAMS_DATA_LENGTH 2
#define COLIBRI_SH_PARAMS_DATA_LENGTH 5

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
constexpr callibri_marker_t CallibriOrientationMarker = 65505;
}

constexpr callibri_marker_t CallibriMaxPacketNumber = 65500;
constexpr std::size_t CallibriPacketSize = 20;
constexpr std::size_t CallibriMarkerPosition = 0;
constexpr std::size_t CallibriMarkerLength = 2;

//Parameters section
static constexpr std::size_t CallibriParamsDataLength = 10;
static constexpr std::size_t CallibriSamplFreqBytePos = 1;
static constexpr std::size_t CallibriAdcInputBytePos = 2;
static constexpr std::size_t CallibriFilterStateBytePos = 3;
static constexpr std::size_t CallibriGainBytePos = 4;
static constexpr std::size_t CallibriOffsetBytePos = 5;
static constexpr std::size_t CallibriExtSwithBytePos = 6;
static constexpr std::size_t CallibriAccelSensBytePos = 8;
static constexpr std::size_t CallibriGyroSensBytePos = 9;

//Command handler section
static constexpr std::size_t CallibriCmdCodePos = 3;
static constexpr std::size_t CallibriAddressPos = 4;
static constexpr std::size_t CallibriChecksumPos = 7;
static constexpr std::size_t CallibriDataStartPos = 8;
static constexpr std::size_t CallibriAddressLength = 3;
static constexpr long CallibriHostAddress = 0xA5B6C7L;

//Signal section
static constexpr std::size_t SignalPacketNumberPos = 0;
static constexpr std::size_t SignalDataShift = 2;

//Mems section
static constexpr std::size_t MemsPacketNumberPos = 2;
static constexpr std::size_t MemsDataShift = 4;

//Respiration section
static constexpr std::size_t RespPacketNumberPos = 2;
static constexpr std::size_t RespDataShift = 4;

//Angle section
static constexpr std::size_t AnglePacketNumberPos = 2;
static constexpr std::size_t OrientationDataShift = 4;


enum class CallibriModule {
    Signal,
    MEMS,
    Stimulator,
    Respiration
};

enum class CallibriPacketType {
    Command,
    Signal,
    MEMS,
    Respiration,
    Orientation
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
    case CallibriOrientationMarker:
        return CallibriPacketType::Orientation;
    case CallibriRespirationMarker:
        return CallibriPacketType::Respiration;
    default:
        throw std::runtime_error("Unresolved marker value");
    }
}

template <typename T>
Byte byteCode(T value){
    return static_cast<Byte>(value);
}

template <typename T>
T fromByteCode(Byte code);

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
inline Gain fromByteCode<Gain>(Byte code){
    switch (code){
    case 0x01:
        return Gain::Gain1;
    case 0x02:
        return Gain::Gain2;
    case 0x03:
        return Gain::Gain3;
    case 0x04:
        return Gain::Gain4;
    case 0x00:
        return Gain::Gain6;
    case 0x05:
        return Gain::Gain8;
    case 0x06:
        return Gain::Gain12;
    default:
        throw std::runtime_error("Unsupported gain byte code");
    }
}

template<>
inline Byte byteCode<SamplingFrequency>(SamplingFrequency value){
    switch (value){
    case SamplingFrequency::Hz125:
        return Byte{0x00};
    case SamplingFrequency::Hz250:
        return Byte{0x01};
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
inline SamplingFrequency fromByteCode<SamplingFrequency>(Byte code){
    switch (code){
    case 0x00:
        return SamplingFrequency::Hz125;
    case 0x01:
        return SamplingFrequency::Hz250;
    case 0x02:
        return SamplingFrequency::Hz500;
    case 0x03:
        return SamplingFrequency::Hz1000;
    case 0x04:
        return SamplingFrequency::Hz2000;
    case 0x05:
        return SamplingFrequency::Hz4000;
    case 0x06:
        return SamplingFrequency::Hz8000;
    default:
        throw std::runtime_error("Unsupported sampling frequency byte code");
    }
}

template<>
inline Byte byteCode<ADCInput>(ADCInput value){
    switch (value){
    case ADCInput::Electrodes:
        return Byte{0x00};
    case ADCInput::Short:
        return Byte{0x01};
    case ADCInput::Test:
        return Byte{0x02};
    case ADCInput::Resistance:
        return Byte{0x03};
    default:
        throw std::runtime_error("Unsupported ADC input value");
    }
}

template<>
inline ADCInput fromByteCode<ADCInput>(Byte code){
    switch (code){
    case 0x00:
        return ADCInput::Electrodes;
    case 0x01:
        return ADCInput::Short;
    case 0x02:
        return ADCInput::Test;
    case 0x03:
        return ADCInput::Resistance;
    default:
        throw std::runtime_error("Unsupported ADC input byte code");
    }
}

template<>
inline Byte byteCode<ExternalSwitchInput>(ExternalSwitchInput value){
    switch (value){
    case ExternalSwitchInput::MioElectrodesRespUSB:
        return Byte{0x00};
    case ExternalSwitchInput::MioElectrodes:
        return Byte{0x01};
    case ExternalSwitchInput::MioUSB:
        return Byte{0x02};
    case ExternalSwitchInput::RespUSB:
        return Byte{0x03};
    default:
        throw std::runtime_error("Unsupported external switch input value");
    }
}

template<>
inline ExternalSwitchInput fromByteCode<ExternalSwitchInput>(Byte code){
    switch (code){
    case 0x00:
        return ExternalSwitchInput::MioElectrodesRespUSB;
    case 0x01:
        return ExternalSwitchInput::MioElectrodes;
    case 0x02:
        return ExternalSwitchInput::MioUSB;
    case 0x03:
        return ExternalSwitchInput::RespUSB;
    default:
        throw std::runtime_error("Unsupported external switch input byte code");
    }
}

template<>
inline Byte byteCode<AccelerometerSensitivity>(AccelerometerSensitivity value){
    switch (value){
    case AccelerometerSensitivity::Sens2g:
        return Byte{0x00};
    case AccelerometerSensitivity::Sens4g:
        return Byte{0x01};
    case AccelerometerSensitivity::Sens8g:
        return Byte{0x02};
    case AccelerometerSensitivity::Sens16g:
        return Byte{0x03};
    default:
        throw std::runtime_error("Unsupported accelerometer sens input value");
    }
}

template<>
inline AccelerometerSensitivity fromByteCode<AccelerometerSensitivity>(Byte code){
    switch (code){
    case 0x00:
        return AccelerometerSensitivity::Sens2g;
    case 0x01:
        return AccelerometerSensitivity::Sens4g;
    case 0x02:
        return AccelerometerSensitivity::Sens8g;
    case 0x03:
        return AccelerometerSensitivity::Sens16g;
    default:
        throw std::runtime_error("Unsupported accelerometer sens input byte code");
    }
}

template<>
inline Byte byteCode<GyroscopeSensitivity>(GyroscopeSensitivity value){
    switch (value){
    case GyroscopeSensitivity::Sens250Grad:
        return Byte{0x00};
    case GyroscopeSensitivity::Sens500Grad:
        return Byte{0x01};
    case GyroscopeSensitivity::Sens1000Grad:
        return Byte{0x02};
    case GyroscopeSensitivity::Sens2000Grad:
        return Byte{0x03};
    default:
        throw std::runtime_error("Unsupported gyroscope sens input value");
    }
}

template<>
inline GyroscopeSensitivity fromByteCode<GyroscopeSensitivity>(Byte code){
    switch (code){
    case 0x00:
        return GyroscopeSensitivity::Sens250Grad;
    case 0x01:
        return GyroscopeSensitivity::Sens500Grad;
    case 0x02:
        return GyroscopeSensitivity::Sens1000Grad;
    case 0x03:
        return GyroscopeSensitivity::Sens2000Grad;
    default:
        throw std::runtime_error("Unsupported gyroscope sens input value");
    }
}

template <Parameter Param>
CallibriCommand toCallibriCommand(){
    switch(Param){
    case Parameter::HardwareFilterState:{
        return CallibriCommand::SWITCH_FILTER_STATE;
    }
    case Parameter::SamplingFrequency:{
        return CallibriCommand::SET_FSAM;
    }
    case Parameter::Gain:{
        return CallibriCommand::SET_PGA_GAIN;
    }
    case Parameter::Offset:{
        return CallibriCommand::SET_DATA_OFFSET;
    }
    case Parameter::ExternalSwitchState:{
        return CallibriCommand::SWITCH_EXT_COM_INPUTS;
    }
    case Parameter::ADCInputState:{
        return CallibriCommand::SWITCH_ADC_INP;
    }
    case Parameter::GyroscopeSens:{
        return CallibriCommand::SET_GYRO_SENS;
    }
    case Parameter::AccelerometerSens:{
        return CallibriCommand::SET_ACCEL_SENS;
    }
    default:
        throw std::runtime_error("Unsupported command type");
    }
}

template <Command Cmd>
CallibriCommand toCallibriCommand(){
    switch(Cmd){
    case Command::StartSignal:{
        return CallibriCommand::START_ADC_DATA_THROW;
    }
    case Command::StopSignal:{
        return CallibriCommand::STOP_ADC_DATA_THROW;
    }
    case Command::StartMEMS:{
        return CallibriCommand::START_MEMS_DATA_THROW;
    }
    case Command::StopMEMS:{
        return CallibriCommand::STOP_MEMS_DATA_THROW;
    }
    case Command::StartRespiration:{
        return CallibriCommand::START_RESP_DATA_THROW;
    }
    case Command::StopRespiration:{
        return CallibriCommand::STOP_RESP_DATA_THROW;
    }
    case Command::StartStimulation:{
        return CallibriCommand::START_STIM;
    }
    case Command::EnableMotionAssistant:{
        return CallibriCommand::SH_START;
    }
    case Command::FindMe:{
        return CallibriCommand::FIND_ME;
    }
    default:
        throw std::runtime_error("Unsupported command type");
    }
}

}
#endif //COLIBRI_PROTOCOL_H
