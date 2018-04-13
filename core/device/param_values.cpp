#include "gsl/gsl_assert"
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
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readName();
}

template<>
typename ParamValue<Parameter::State>::Type Device::readParam<Parameter::State>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readState();
}

template<>
typename Neuro::ParamValue<Parameter::Address>::Type Neuro::Device::readParam<Parameter::Address>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readAddress();
}

template<>
typename ParamValue<Parameter::SerialNumber>::Type Device::readParam<Parameter::SerialNumber>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readSerialNumber();
}

template<>
typename ParamValue<Parameter::HardwareFilterState>::Type Device::readParam<Parameter::HardwareFilterState>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readHardwareFilterState();
}

template<>
typename ParamValue<Parameter::FirmwareMode>::Type Device::readParam<Parameter::FirmwareMode>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readFirmwareMode();
}

template<>
typename ParamValue<Parameter::SamplingFrequency>::Type Device::readParam<Parameter::SamplingFrequency>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readSamplingFrequency();
}

template<>
typename ParamValue<Parameter::Gain>::Type Device::readParam<Parameter::Gain>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readGain();
}

template<>
typename ParamValue<Parameter::Offset>::Type Device::readParam<Parameter::Offset>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readOffset();
}

template<>
typename ParamValue<Parameter::ExternalSwitchState>::Type Device::readParam<Parameter::ExternalSwitchState>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readExternalSwitchState();
}

template<>
typename ParamValue<Parameter::ADCInputState>::Type Device::readParam<Parameter::ADCInputState>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readADCInputState();
}

template<>
typename ParamValue<Parameter::StimulatorState>::Type Device::readParam<Parameter::StimulatorState>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readStimulatorState();
}

template<>
typename ParamValue<Parameter::MotionAssistantState>::Type Device::readParam<Parameter::MotionAssistantState>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readMotionAssistantState();
}

template<>
typename ParamValue<Parameter::StimulatorParamPack>::Type Device::readParam<Parameter::StimulatorParamPack>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readStimulatorParamPack();
}

template<>
typename ParamValue<Parameter::MotionAssistantParamPack>::Type Device::readParam<Parameter::MotionAssistantParamPack>() const {
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamReader != nullptr);
    return mImpl->mParamReader->readMotionAssistantParamPack();
}

/*
 * Parameters set
 */
template<>
bool Device::setParam<Parameter::Name>(typename ParamValue<Parameter::Name>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setName(value);
}

template<>
bool Device::setParam<Parameter::State>(typename ParamValue<Parameter::State>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setState(value);
}

template<>
bool Device::setParam<Parameter::Address>(typename ParamValue<Parameter::Address>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setAddress(value);
}

template<>
bool Device::setParam<Parameter::SerialNumber>(typename ParamValue<Parameter::SerialNumber>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setSerialNumber(value);
}

template<>
bool Device::setParam<Parameter::HardwareFilterState>(typename ParamValue<Parameter::HardwareFilterState>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setHardwareFilterState(value);
}

template<>
bool Device::setParam<Parameter::FirmwareMode>(typename ParamValue<Parameter::FirmwareMode>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setFirmwareMode(value);
}

template<>
bool Device::setParam<Parameter::SamplingFrequency>(typename ParamValue<Parameter::SamplingFrequency>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setSamplingFrequency(value);
}

template<>
bool Device::setParam<Parameter::Gain>(typename ParamValue<Parameter::Gain>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setGain(value);
}

template<>
bool Device::setParam<Parameter::Offset>(typename ParamValue<Parameter::Offset>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setOffset(value);
}

template<>
bool Device::setParam<Parameter::ExternalSwitchState>(typename ParamValue<Parameter::ExternalSwitchState>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setExternalSwitchState(value);
}

template<>
bool Device::setParam<Parameter::ADCInputState>(typename ParamValue<Parameter::ADCInputState>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setADCInputState(value);
}

template<>
bool Device::setParam<Parameter::StimulatorState>(typename ParamValue<Parameter::StimulatorState>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setStimulatorState(value);
}

template<>
bool Device::setParam<Parameter::MotionAssistantState>(typename ParamValue<Parameter::MotionAssistantState>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setMotionAssistantState(value);
}

template<>
bool Device::setParam<Parameter::StimulatorParamPack>(typename ParamValue<Parameter::StimulatorParamPack>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setStimulatorParamPack(value);
}

template<>
bool Device::setParam<Parameter::MotionAssistantParamPack>(typename ParamValue<Parameter::MotionAssistantParamPack>::Type value){
    Expects(mImpl != nullptr);
    Expects(mImpl->mParamWriter != nullptr);
    return mImpl->mParamWriter->setMotionAssistantParamPack(value);
}




}
