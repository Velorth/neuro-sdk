#ifndef PARAMETER_WRITER_H
#define PARAMETER_WRITER_H

#include "param_values.h"

namespace Neuro {

class ParameterWriter{
public:
    virtual ~ParameterWriter() = default;

    bool setName(typename ParamValue<Parameter::Name>::Type);
    bool setState(typename ParamValue<Parameter::State>::Type);
    bool setAddress(typename ParamValue<Parameter::Address>::Type);
    virtual
    bool setSerialNumber(typename ParamValue<Parameter::SerialNumber>::Type) = 0;
    virtual
    bool setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type) = 0;
    virtual
    bool setFirmwareMode(typename ParamValue<Parameter::FirmwareMode>::Type) = 0;
    virtual
    bool setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type) = 0;
    virtual
    bool setGain(typename ParamValue<Parameter::Gain>::Type) = 0;
    virtual
    bool setOffset(typename ParamValue<Parameter::Offset>::Type) = 0;
    virtual
    bool setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type) = 0;
    virtual
    bool setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type) = 0;
    virtual
    bool setAccelerometerSens(typename ParamValue<Parameter::AccelerometerSens>::Type) = 0;
    virtual
    bool setGyroscopeSens(typename ParamValue<Parameter::GyroscopeSens>::Type) = 0;
    virtual
    bool setStimulatorState(typename ParamValue<Parameter::StimulatorState>::Type) = 0;
    virtual
    bool setMotionAssistantState(typename ParamValue<Parameter::MotionAssistantState>::Type) = 0;
    virtual
    bool setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type) = 0;
    virtual
    bool setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type) = 0;
    virtual
    bool setFirmwareVersion(typename ParamValue<Parameter::FirmwareVersion>::Type) = 0;
};

}

#endif // PARAMETER_WRITER_H
