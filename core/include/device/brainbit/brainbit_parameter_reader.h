#ifndef BRAINBIT_PARAMETER_READER_H
#define BRAINBIT_PARAMETER_READER_H

#include "device/parameter_reader.h"

namespace Neuro {

class BrainbitParameterReader : public ParameterReader {
public:
    BrainbitParameterReader(std::shared_ptr<BleDevice>);

    ParamValue<Parameter::SerialNumber>::Type readSerialNumber() const override;
    ParamValue<Parameter::HardwareFilterState>::Type readHardwareFilterState() const override;
    ParamValue<Parameter::FirmwareMode>::Type readFirmwareMode() const override;
    ParamValue<Parameter::SamplingFrequency>::Type readSamplingFrequency() const override;
    ParamValue<Parameter::Gain>::Type readGain() const override;
    ParamValue<Parameter::Offset>::Type readOffset() const override;
    ParamValue<Parameter::ExternalSwitchState>::Type readExternalSwitchState() const override;
    ParamValue<Parameter::ADCInputState>::Type readADCInputState() const override;
    ParamValue<Parameter::StimulatorState>::Type readStimulatorState() const override;
    ParamValue<Parameter::MotionAssistantState>::Type readMotionAssistantState() const override;
    ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const override;
    ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const override;

private:
    bool loadDeviceParams() override;
};

}

#endif // BRAINBIT_PARAMETER_READER_H
