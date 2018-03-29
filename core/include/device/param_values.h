#ifndef PARAM_VALUES_H
#define PARAM_VALUES_H

#include <string>
#include "device.h"

namespace Neuro {
template<>
struct ParamValue<Parameter::Name>{
    using Type = std::string;
};

template<>
struct ParamValue<Parameter::State>{
    using Type = DeviceState;
};

template<>
struct ParamValue<Parameter::Address>{
    using Type = std::string;
};

template<>
struct ParamValue<Parameter::SerialNumber>{
    using Type = std::string;
};

template<>
struct ParamValue<Parameter::HardwareFilterState>{
    using Type = bool;
};

template<>
struct ParamValue<Parameter::FirmwareMode>{
    using Type = FirmwareMode;
};

template<>
struct ParamValue<Parameter::SamplingFrequency>{
    using Type = SamplingFrequency;
};

template<>
struct ParamValue<Parameter::Gain>{
    using Type = Gain;
};

template<>
struct ParamValue<Parameter::Offset>{//0-8
    using Type = unsigned char;
};

template<>
struct ParamValue<Parameter::ExternalSwitchState>{
    using Type = ExternalSwitchInput;
};

template<>
struct ParamValue<Parameter::ADCInputState>{
    using Type = ADCInput;
};

template<>
struct ParamValue<Parameter::AccelerometerSens>{
    using Type = AccelerometerSensitivity;
};

template<>
struct ParamValue<Parameter::GyroscopeSens>{
    using Type = GyroscopeSensitivity;
};

template<>
struct ParamValue<Parameter::StimulatorState>{
    using Type = bool;
};

template<>
struct ParamValue<Parameter::MotionAssistantState>{
    using Type = bool;
};

template<>
struct ParamValue<Parameter::StimulatorParamPack>{
    using Type = StimulationParams;
};

template<>
struct ParamValue<Parameter::MotionAssistantParamPack>{
    using Type = MotionAssistantParams;
};

/*
 * Parameters read
 */
template<>
typename ParamValue<Parameter::Name>::Type Device::readParam<Parameter::Name>() const;
template<>
typename ParamValue<Parameter::State>::Type Device::readParam<Parameter::State>() const;
template<>
typename ParamValue<Parameter::Address>::Type Device::readParam<Parameter::Address>() const;
template<>
typename ParamValue<Parameter::SerialNumber>::Type Device::readParam<Parameter::SerialNumber>() const;
template<>
typename ParamValue<Parameter::HardwareFilterState>::Type Device::readParam<Parameter::HardwareFilterState>() const;
template<>
typename ParamValue<Parameter::FirmwareMode>::Type Device::readParam<Parameter::FirmwareMode>() const;
template<>
typename ParamValue<Parameter::SamplingFrequency>::Type Device::readParam<Parameter::SamplingFrequency>() const;
template<>
typename ParamValue<Parameter::Gain>::Type Device::readParam<Parameter::Gain>() const;
template<>
typename ParamValue<Parameter::Offset>::Type Device::readParam<Parameter::Offset>() const;
template<>
typename ParamValue<Parameter::ExternalSwitchState>::Type Device::readParam<Parameter::ExternalSwitchState>() const;
template<>
typename ParamValue<Parameter::ADCInputState>::Type Device::readParam<Parameter::ADCInputState>() const;
template<>
typename ParamValue<Parameter::AccelerometerSens>::Type Device::readParam<Parameter::AccelerometerSens>() const;
template<>
typename ParamValue<Parameter::GyroscopeSens>::Type Device::readParam<Parameter::GyroscopeSens>() const;
template<>
typename ParamValue<Parameter::StimulatorState>::Type Device::readParam<Parameter::StimulatorState>() const;
template<>
typename ParamValue<Parameter::MotionAssistantState>::Type Device::readParam<Parameter::MotionAssistantState>() const;
template<>
typename ParamValue<Parameter::StimulatorParamPack>::Type Device::readParam<Parameter::StimulatorParamPack>() const;
template<>
typename ParamValue<Parameter::MotionAssistantParamPack>::Type Device::readParam<Parameter::MotionAssistantParamPack>() const;

/*
 * Parameters set
 */
template<>
bool Device::setParam<Parameter::Name>(typename ParamValue<Parameter::Name>::Type);
template<>
bool Device::setParam<Parameter::State>(typename ParamValue<Parameter::State>::Type);
template<>
bool Device::setParam<Parameter::Address>(typename ParamValue<Parameter::Address>::Type);
template<>
bool Device::setParam<Parameter::SerialNumber>(typename ParamValue<Parameter::SerialNumber>::Type);
template<>
bool Device::setParam<Parameter::HardwareFilterState>(typename ParamValue<Parameter::HardwareFilterState>::Type);
template<>
bool Device::setParam<Parameter::FirmwareMode>(typename ParamValue<Parameter::FirmwareMode>::Type);
template<>
bool Device::setParam<Parameter::SamplingFrequency>(typename ParamValue<Parameter::SamplingFrequency>::Type);
template<>
bool Device::setParam<Parameter::Gain>(typename ParamValue<Parameter::Gain>::Type);
template<>
bool Device::setParam<Parameter::Offset>(typename ParamValue<Parameter::Offset>::Type);
template<>
bool Device::setParam<Parameter::ExternalSwitchState>(typename ParamValue<Parameter::ExternalSwitchState>::Type);
template<>
bool Device::setParam<Parameter::ADCInputState>(typename ParamValue<Parameter::ADCInputState>::Type);
template<>
bool Device::setParam<Parameter::AccelerometerSens>(typename ParamValue<Parameter::AccelerometerSens>::Type);
template<>
bool Device::setParam<Parameter::GyroscopeSens>(typename ParamValue<Parameter::GyroscopeSens>::Type);
template<>
bool Device::setParam<Parameter::StimulatorState>(typename ParamValue<Parameter::StimulatorState>::Type);
template<>
bool Device::setParam<Parameter::MotionAssistantState>(typename ParamValue<Parameter::MotionAssistantState>::Type);
template<>
bool Device::setParam<Parameter::StimulatorParamPack>(typename ParamValue<Parameter::StimulatorParamPack>::Type);
template<>
bool Device::setParam<Parameter::MotionAssistantParamPack>(typename ParamValue<Parameter::MotionAssistantParamPack>::Type);

}

#endif // PARAM_VALUES_H
