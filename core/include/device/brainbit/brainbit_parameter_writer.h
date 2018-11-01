#ifndef BRAINBIT_PARAMETER_WRITER_H
#define BRAINBIT_PARAMETER_WRITER_H

#include "device/parameter_writer.h"

namespace Neuro {

class BrainbitParameterWriter : public ParameterWriter {
public:
    bool setSerialNumber(typename ParamValue<Parameter::SerialNumber>::Type) override;
    bool setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type) override;
    bool setFirmwareMode(typename ParamValue<Parameter::FirmwareMode>::Type) override;
    bool setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type) override;
    bool setGain(typename ParamValue<Parameter::Gain>::Type) override;
    bool setOffset(typename ParamValue<Parameter::Offset>::Type) override;
    bool setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type) override;
    bool setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type) override;
    bool setAccelerometerSens(ParamValue<Parameter::AccelerometerSens>::Type) override;
    bool setGyroscopeSens(ParamValue<Parameter::GyroscopeSens>::Type) override;
    bool setStimulatorState(typename ParamValue<Parameter::StimulatorState>::Type) override;
    bool setMotionAssistantState(typename ParamValue<Parameter::MotionAssistantState>::Type) override;
    bool setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type) override;
    bool setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type) override;
    bool setFirmwareVersion(typename ParamValue<Parameter::FirmwareVersion>::Type) override;
};

}

#endif // BRAINBIT_PARAMETER_WRITER_H
