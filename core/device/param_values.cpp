#include "device/param_values.h"
#include "device/device_impl.h"
#include "device/parameter_reader.h"
#include "device/parameter_writer.h"

namespace Neuro {

/*
 * Parameters read
 */
template<>
typename ParamValue<Parameter::Name>::Type Device::readParam<Parameter::Name>() const {
    return mImpl->mParamReader->readName();
}

template<>
typename ParamValue<Parameter::State>::Type Device::readParam<Parameter::State>() const {
    return mImpl->mParamReader->readState();
}

template<>
typename Neuro::ParamValue<Parameter::Address>::Type Neuro::Device::readParam<Parameter::Address>() const {
    return mImpl->mParamReader->readAddress();
}

template<>
typename ParamValue<Parameter::SerialNumber>::Type Device::readParam<Parameter::SerialNumber>() const {
    return mImpl->mParamReader->readSerialNumber();
}

template<>
typename ParamValue<Parameter::HardwareFilterState>::Type Device::readParam<Parameter::HardwareFilterState>() const {
    return mImpl->mParamReader->readHardwareFilterState();
}

template<>
typename ParamValue<Parameter::FirmwareMode>::Type Device::readParam<Parameter::FirmwareMode>() const {
    return mImpl->mParamReader->readFirmwareMode();
}

template<>
typename ParamValue<Parameter::SamplingFrequency>::Type Device::readParam<Parameter::SamplingFrequency>() const {
    return mImpl->mParamReader->readSamplingFrequency();
}

template<>
typename ParamValue<Parameter::Gain>::Type Device::readParam<Parameter::Gain>() const {
    return mImpl->mParamReader->readGain();
}

template<>
typename ParamValue<Parameter::Offset>::Type Device::readParam<Parameter::Offset>() const {
    return mImpl->mParamReader->readOffset();
}

template<>
typename ParamValue<Parameter::ExternalSwitchState>::Type Device::readParam<Parameter::ExternalSwitchState>() const {
    return mImpl->mParamReader->readExternalSwitchState();
}

template<>
typename ParamValue<Parameter::ADCInputState>::Type Device::readParam<Parameter::ADCInputState>() const {
    return mImpl->mParamReader->readADCInputState();
}

template<>
typename ParamValue<Parameter::GyroscopeSens>::Type Device::readParam<Parameter::GyroscopeSens>() const {
    return mImpl->mParamReader->readGyroscopeSens();
}

template<>
typename ParamValue<Parameter::AccelerometerSens>::Type Device::readParam<Parameter::AccelerometerSens>() const {
    return mImpl->mParamReader->readAccelerometerSens();
}

template<>
typename ParamValue<Parameter::StimulatorAndMAState>::Type Device::readParam<Parameter::StimulatorAndMAState>() const {
    return mImpl->mParamReader->readStimulatorAndMAState();
}

template<>
typename ParamValue<Parameter::StimulatorParamPack>::Type Device::readParam<Parameter::StimulatorParamPack>() const {
    return mImpl->mParamReader->readStimulatorParamPack();
}

template<>
typename ParamValue<Parameter::MotionAssistantParamPack>::Type Device::readParam<Parameter::MotionAssistantParamPack>() const {
    return mImpl->mParamReader->readMotionAssistantParamPack();
}

template<>
typename ParamValue<Parameter::FirmwareVersion>::Type Device::readParam<Parameter::FirmwareVersion>() const {
	return mImpl->mParamReader->readFirmwareVersion();
}

/*
 * Parameters set
 */
template<>
bool Device::setParam<Parameter::Name>(typename ParamValue<Parameter::Name>::Type value){
    return mImpl->mParamWriter->setName(value);
}

template<>
bool Device::setParam<Parameter::State>(typename ParamValue<Parameter::State>::Type value){
    return mImpl->mParamWriter->setState(value);
}

template<>
bool Device::setParam<Parameter::Address>(typename ParamValue<Parameter::Address>::Type value){
    return mImpl->mParamWriter->setAddress(value);
}

template<>
bool Device::setParam<Parameter::SerialNumber>(typename ParamValue<Parameter::SerialNumber>::Type value){
    return mImpl->mParamWriter->setSerialNumber(value);
}

template<>
bool Device::setParam<Parameter::HardwareFilterState>(typename ParamValue<Parameter::HardwareFilterState>::Type value){
    return mImpl->mParamWriter->setHardwareFilterState(value);
}

template<>
bool Device::setParam<Parameter::FirmwareMode>(typename ParamValue<Parameter::FirmwareMode>::Type value){
    return mImpl->mParamWriter->setFirmwareMode(value);
}

template<>
bool Device::setParam<Parameter::SamplingFrequency>(typename ParamValue<Parameter::SamplingFrequency>::Type value){
    return mImpl->mParamWriter->setSamplingFrequency(value);
}

template<>
bool Device::setParam<Parameter::Gain>(typename ParamValue<Parameter::Gain>::Type value){
    return mImpl->mParamWriter->setGain(value);
}

template<>
bool Device::setParam<Parameter::Offset>(typename ParamValue<Parameter::Offset>::Type value){
    return mImpl->mParamWriter->setOffset(value);
}

template<>
bool Device::setParam<Parameter::ExternalSwitchState>(typename ParamValue<Parameter::ExternalSwitchState>::Type value){
    return mImpl->mParamWriter->setExternalSwitchState(value);
}

template<>
bool Device::setParam<Parameter::ADCInputState>(typename ParamValue<Parameter::ADCInputState>::Type value){
    return mImpl->mParamWriter->setADCInputState(value);
}

template<>
bool Device::setParam<Parameter::GyroscopeSens>(typename ParamValue<Parameter::GyroscopeSens>::Type value){
    return mImpl->mParamWriter->setGyroscopeSens(value);
}

template<>
bool Device::setParam<Parameter::AccelerometerSens>(typename ParamValue<Parameter::AccelerometerSens>::Type value){
    return mImpl->mParamWriter->setAccelerometerSens(value);
}

template<>
bool Device::setParam<Parameter::StimulatorParamPack>(typename ParamValue<Parameter::StimulatorParamPack>::Type value){
    return mImpl->mParamWriter->setStimulatorParamPack(value);
}

template<>
bool Device::setParam<Parameter::MotionAssistantParamPack>(typename ParamValue<Parameter::MotionAssistantParamPack>::Type value){
    return mImpl->mParamWriter->setMotionAssistantParamPack(value);
}

template<>
bool Device::setParam<Parameter::FirmwareVersion>(typename ParamValue<Parameter::FirmwareVersion>::Type value){
    return mImpl->mParamWriter->setFirmwareVersion(value);
}

}
