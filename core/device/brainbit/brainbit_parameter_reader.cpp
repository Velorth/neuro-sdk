#include "device/brainbit_parameter_reader.h"

namespace Neuro {

BrainbitParameterReader::BrainbitParameterReader(std::shared_ptr<BleDevice> ble_device):
    ParameterReader(ble_device){

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

typename ParamValue<Parameter::StimulatorState>::Type
BrainbitParameterReader::readStimulatorState() const {
    throw std::runtime_error("Attempt to read unsupported parameter");
}

typename ParamValue<Parameter::MotionAssistantState>::Type
BrainbitParameterReader::readMotionAssistantState() const {
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

bool BrainbitParameterReader::loadDeviceParams(){
    return true;
}

}
