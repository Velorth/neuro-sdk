#ifndef BRAINBIT_PARAMETER_READER_H
#define BRAINBIT_PARAMETER_READER_H

#include "device/parameter_reader.h"

namespace Neuro {

class BrainbitParameterReader : public ParameterReader {
public:
    BrainbitParameterReader(std::shared_ptr<BleDevice>, param_changed_callback_t);

    typename ParamValue<Parameter::SerialNumber>::Type readSerialNumber() const override;
    typename ParamValue<Parameter::HardwareFilterState>::Type readHardwareFilterState() const override;
    typename ParamValue<Parameter::FirmwareMode>::Type readFirmwareMode() const override;
    typename ParamValue<Parameter::SamplingFrequency>::Type readSamplingFrequency() const override;
    typename ParamValue<Parameter::Gain>::Type readGain() const override;
    typename ParamValue<Parameter::Offset>::Type readOffset() const override;
    typename ParamValue<Parameter::ExternalSwitchState>::Type readExternalSwitchState() const override;
    typename ParamValue<Parameter::ADCInputState>::Type readADCInputState() const override;
    typename ParamValue<Parameter::AccelerometerSens>::Type readAccelerometerSens() const override;
    typename ParamValue<Parameter::GyroscopeSens>::Type readGyroscopeSens() const override;
    typename ParamValue<Parameter::StimulatorState>::Type readStimulatorState() const override;
    typename ParamValue<Parameter::MotionAssistantState>::Type readMotionAssistantState() const override;
    typename ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const override;
    typename ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const override;

private:
    bool loadDeviceParams() override;
};

}

#endif // BRAINBIT_PARAMETER_READER_H
