#ifndef CALLIBRI_PARAMETER_WRITER_H
#define CALLIBRI_PARAMETER_WRITER_H

#include "device/parameter_writer.h"

namespace Neuro {

class CallibriCommonParameters;

class CallibriParameterWriter : public ParameterWriter {
public:
    CallibriParameterWriter(std::shared_ptr<CallibriCommonParameters>);

    bool setSerialNumber(typename ParamValue<Parameter::SerialNumber>::Type) override;
    bool setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type) override;
    bool setFirmwareMode(typename ParamValue<Parameter::FirmwareMode>::Type) override;
    bool setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type) override;
    bool setGain(typename ParamValue<Parameter::Gain>::Type) override;
    bool setOffset(typename ParamValue<Parameter::Offset>::Type) override;
    bool setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type) override;
    bool setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type) override;
    bool setAccelerometerSens(typename ParamValue<Parameter::AccelerometerSens>::Type) override;
    bool setGyroscopeSens(typename ParamValue<Parameter::GyroscopeSens>::Type) override;
    bool setStimulatorState(typename ParamValue<Parameter::StimulatorState>::Type) override;
    bool setMotionAssistantState(typename ParamValue<Parameter::MotionAssistantState>::Type) override;
    bool setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type) override;
    bool setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type) override;

private:
    std::shared_ptr<CallibriCommonParameters> mCommonParameters;
};

}

#endif // CALLIBRI_PARAMETER_WRITER_H
