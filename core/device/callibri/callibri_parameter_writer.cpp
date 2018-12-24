#include <saturation_cast.h>
#include "device/callibri/callibri_parameter_writer.h"

namespace Neuro {

CallibriParameterWriter::CallibriParameterWriter(std::shared_ptr<CallibriCommonParameters> common_params,
                                                 std::shared_ptr<CallibriRequestScheduler> request_handler) :
    mCommonParameters(common_params),
    mRequestHandler(request_handler){

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

bool CallibriParameterWriter::setStimulatorParamPack(typename ParamValue<Parameter::StimulatorParamPack>::Type stimulationParams){
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::GET_STIM_PARAM);
    ByteInterpreter<unsigned short> impulseWidth;
    impulseWidth.value = saturation_cast<unsigned short>(stimulationParams.stimulus_duration);
    cmdData->setRequestData({1,
                             saturation_cast<Byte>(stimulationParams.current),
                             saturation_cast<Byte>(stimulationParams.pulse_width/10),
                             saturation_cast<Byte>(stimulationParams.frequency),
                             impulseWidth.bytes[0],
                             impulseWidth.bytes[1]});
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == CallibriError::NO_ERROR;
}

bool CallibriParameterWriter::setMotionAssistantParamPack(typename ParamValue<Parameter::MotionAssistantParamPack>::Type maParams){
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::GET_SH_PARAM);
    cmdData->setRequestData({0,
                             static_cast<Byte>(maParams.gyroStart),
                             static_cast<Byte>(maParams.gyroStop),
                             static_cast<Byte>(maParams.limb),
                             static_cast<Byte>(maParams.minPause / 10)
                            });
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == CallibriError::NO_ERROR;
}

bool CallibriParameterWriter::setFirmwareVersion(typename ParamValue<Parameter::FirmwareVersion>::Type){
    throw std::runtime_error("Unable to set readonly parameter");
}

}
