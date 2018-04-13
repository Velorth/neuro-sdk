#include "device/brainbit_parameter_writer.h"

namespace Neuro {

bool BrainbitParameterWriter::setSerialNumber(typename ParamValue<Parameter::SerialNumber>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setFirmwareMode(typename ParamValue<Parameter::FirmwareMode>::Type){
    return false;
}

bool BrainbitParameterWriter::setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool BrainbitParameterWriter::setGain(typename ParamValue<Parameter::Gain>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool BrainbitParameterWriter::setOffset(typename ParamValue<Parameter::Offset>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

bool BrainbitParameterWriter::setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setStimulatorState(typename ParamValue<Parameter::StimulatorState>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setMotionAssistantState(typename ParamValue<Parameter::MotionAssistantState>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type){
    throw std::runtime_error("Device parameter not found");
}

bool BrainbitParameterWriter::setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type){
    throw std::runtime_error("Device parameter not found");
}



}
