#include "device/callibri/callibri_parameter_writer.h"
#include "device/callibri/callibri_common_parameters.h"

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

bool CallibriParameterWriter::setAccelerometerSens(typename ParamValue<Parameter::AccelerometerSens>::Type value){
    return mCommonParameters->setAccelerometerSens(value);
}

bool CallibriParameterWriter::setGyroscopeSens(typename ParamValue<Parameter::GyroscopeSens>::Type value){
    return mCommonParameters->setGyroscopeSens(value);
}

bool CallibriParameterWriter::setStimulatorState(typename ParamValue<Parameter::StimulatorState>::Type value){
    throw std::runtime_error("Not implemented exception");
}

bool CallibriParameterWriter::setMotionAssistantState(typename ParamValue<Parameter::MotionAssistantState>::Type value){
    throw std::runtime_error("Not implemented exception");
}

bool CallibriParameterWriter::setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type value){
    throw std::runtime_error("Not implemented exception");
}

bool CallibriParameterWriter::setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type value){
    throw std::runtime_error("Not implemented exception");
}

}
