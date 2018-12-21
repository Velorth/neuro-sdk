#ifndef BRAINBIT_PARAMETER_READER_H
#define BRAINBIT_PARAMETER_READER_H

#include "device/parameter_reader.h"
#include "brainbit_impl.h"

namespace Neuro {

class BrainbitParameterReader : public ParameterReader {
public:
    BrainbitParameterReader(std::shared_ptr<BleDevice>, param_changed_callback_t, const BrainbitImpl::BrainbitParameterSetter &);

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
    typename ParamValue<Parameter::StimulatorAndMAState>::Type readStimulatorAndMAState() const override;
    typename ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const override;
    typename ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const override;
	typename ParamValue<Parameter::FirmwareVersion>::Type readFirmwareVersion() const override;

private:
    bool loadDeviceParams() override;
	const BrainbitImpl::BrainbitParameterSetter &mSetter;
};

}

#endif // BRAINBIT_PARAMETER_READER_H
