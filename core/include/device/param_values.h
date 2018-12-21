#ifndef PARAM_VALUES_H
#define PARAM_VALUES_H

#include <string>
#include "device.h"
#include "lib_export.h"

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
struct ParamValue<Parameter::StimulatorAndMAState>{
    using Type = StimulatorDeviceState;
};

template<>
struct ParamValue<Parameter::StimulatorParamPack>{
    using Type = StimulationParams;
};

template<>
struct ParamValue<Parameter::MotionAssistantParamPack>{
    using Type = MotionAssistantParams;
};

template<>
struct ParamValue<Parameter::FirmwareVersion> {
	using Type = FirmwareVersion;
};

/*
 * Parameters read
 */
template<>
SDK_SHARED typename ParamValue<Parameter::Name>::Type Device::readParam<Parameter::Name>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::State>::Type Device::readParam<Parameter::State>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::Address>::Type Device::readParam<Parameter::Address>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::SerialNumber>::Type Device::readParam<Parameter::SerialNumber>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::HardwareFilterState>::Type Device::readParam<Parameter::HardwareFilterState>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::FirmwareMode>::Type Device::readParam<Parameter::FirmwareMode>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::SamplingFrequency>::Type Device::readParam<Parameter::SamplingFrequency>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::Gain>::Type Device::readParam<Parameter::Gain>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::Offset>::Type Device::readParam<Parameter::Offset>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::ExternalSwitchState>::Type Device::readParam<Parameter::ExternalSwitchState>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::ADCInputState>::Type Device::readParam<Parameter::ADCInputState>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::AccelerometerSens>::Type Device::readParam<Parameter::AccelerometerSens>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::GyroscopeSens>::Type Device::readParam<Parameter::GyroscopeSens>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::StimulatorAndMAState>::Type Device::readParam<Parameter::StimulatorAndMAState>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::StimulatorParamPack>::Type Device::readParam<Parameter::StimulatorParamPack>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::MotionAssistantParamPack>::Type Device::readParam<Parameter::MotionAssistantParamPack>() const;
template<>
SDK_SHARED typename ParamValue<Parameter::FirmwareVersion>::Type Device::readParam<Parameter::FirmwareVersion>() const;

/*
 * Parameters set
 */
template<>
SDK_SHARED bool Device::setParam<Parameter::Name>(typename ParamValue<Parameter::Name>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::State>(typename ParamValue<Parameter::State>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::Address>(typename ParamValue<Parameter::Address>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::SerialNumber>(typename ParamValue<Parameter::SerialNumber>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::HardwareFilterState>(typename ParamValue<Parameter::HardwareFilterState>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::FirmwareMode>(typename ParamValue<Parameter::FirmwareMode>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::SamplingFrequency>(typename ParamValue<Parameter::SamplingFrequency>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::Gain>(typename ParamValue<Parameter::Gain>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::Offset>(typename ParamValue<Parameter::Offset>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::ExternalSwitchState>(typename ParamValue<Parameter::ExternalSwitchState>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::ADCInputState>(typename ParamValue<Parameter::ADCInputState>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::AccelerometerSens>(typename ParamValue<Parameter::AccelerometerSens>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::GyroscopeSens>(typename ParamValue<Parameter::GyroscopeSens>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::StimulatorParamPack>(typename ParamValue<Parameter::StimulatorParamPack>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::MotionAssistantParamPack>(typename ParamValue<Parameter::MotionAssistantParamPack>::Type);
template<>
SDK_SHARED bool Device::setParam<Parameter::FirmwareVersion>(typename ParamValue<Parameter::FirmwareVersion>::Type);

}

#endif // PARAM_VALUES_H
