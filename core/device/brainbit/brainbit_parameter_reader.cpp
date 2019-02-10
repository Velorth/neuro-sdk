#include "device/brainbit/brainbit_parameter_reader.h"

namespace Neuro {

BrainbitParameterReader::BrainbitParameterReader(std::shared_ptr<BleDevice> ble_device,
                                                 param_changed_callback_t callback,
												 const BrainbitImpl::BrainbitParameterSetter &setter):
    ParameterReader(ble_device, callback),
	mSetter(setter){

}

typename ParamValue<Parameter::SerialNumber>::Type
BrainbitParameterReader::readSerialNumber() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::HardwareFilterState>::Type
BrainbitParameterReader::readHardwareFilterState() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::FirmwareMode>::Type
BrainbitParameterReader::readFirmwareMode() const {
    return FirmwareMode::Application;
}

typename ParamValue<Parameter::SamplingFrequency>::Type
BrainbitParameterReader::readSamplingFrequency() const {
    return SamplingFrequency::Hz250;
}

typename ParamValue<Parameter::Gain>::Type
BrainbitParameterReader::readGain() const {
    return Gain::Gain6;
}

typename ParamValue<Parameter::Offset>::Type
BrainbitParameterReader::readOffset() const {
    return 0;
}

typename ParamValue<Parameter::ExternalSwitchState>::Type
BrainbitParameterReader::readExternalSwitchState() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::ADCInputState>::Type
BrainbitParameterReader::readADCInputState() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::AccelerometerSens>::Type
BrainbitParameterReader::readAccelerometerSens() const{
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::GyroscopeSens>::Type
BrainbitParameterReader::readGyroscopeSens() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::StimulatorAndMAState>::Type
BrainbitParameterReader::readStimulatorAndMAState() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::StimulatorParamPack>::Type
BrainbitParameterReader::readStimulatorParamPack() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::MotionAssistantParamPack>::Type
BrainbitParameterReader::readMotionAssistantParamPack() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::FirmwareVersion>::Type
BrainbitParameterReader::readFirmwareVersion() const {
	return mSetter.mFirmwareVersion;
}

bool BrainbitParameterReader::loadDeviceParams(){
    return true;
}

}
