#include "device/callibri_parameter_writer.h"
#include "device/callibri_common_parameters.h"

namespace Neuro {

CallibriParameterWriter::CallibriParameterWriter(std::shared_ptr<CallibriCommonParameters> common_params) :
    mCommonParameters(common_params){

}

bool CallibriParameterWriter::setSerialNumber(typename ParamValue<Parameter::SerialNumber>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool CallibriParameterWriter::setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type value){
    return mCommonParameters->setHardwareFilterState(value);
}

bool CallibriParameterWriter::setFirmwareMode(typename ParamValue<Parameter::FirmwareMode>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool CallibriParameterWriter::setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type value){
    return mCommonParameters->setSamplingFrequency(value);
}

bool CallibriParameterWriter::setGain(typename ParamValue<Parameter::Gain>::Type value){
    return mCommonParameters->setGain(value);
}

bool CallibriParameterWriter::setOffset(typename ParamValue<Parameter::Offset>::Type value){
    return mCommonParameters->setOffset(value);
}

bool CallibriParameterWriter::setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type value){
    return mCommonParameters->setExternalSwitchState(value);
}

bool CallibriParameterWriter::setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type value){
    return mCommonParameters->setADCInputState(value);
}

bool CallibriParameterWriter::setStimulatorState(typename ParamValue<Parameter::StimulatorState>::Type value){

}

bool CallibriParameterWriter::setMotionAssistantState(typename ParamValue<Parameter::MotionAssistantState>::Type value){

}

bool CallibriParameterWriter::setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type value){

}

bool CallibriParameterWriter::setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type value){

}

}
