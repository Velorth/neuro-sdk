#include "device/callibri/callibri_parameter_reader.h"
#include "device/callibri/callibri_common_parameters.h"
#include "device/callibri/callibri_command.h"
#include "device/callibri/callibri_protocol.h"
#include "device/request_handler.h"
#include "common_types.h"

namespace Neuro {

CallibriParameterReader::CallibriParameterReader(std::shared_ptr<BleDevice> ble_device,
                                                 std::shared_ptr<CallibriCommonParameters> common_params,
                                                 std::shared_ptr<CallibriRequestHandler> request_handler) :
    ParameterReader(ble_device),
    mCommonParameters(common_params),
    mRequestHandler(request_handler){

}

typename ParamValue<Parameter::SerialNumber>::Type
CallibriParameterReader::readSerialNumber() const {
    return std::to_string(mCommonParameters->callibriAddress());
}

typename ParamValue<Parameter::HardwareFilterState>::Type
CallibriParameterReader::readHardwareFilterState() const {
    return mCommonParameters->hardwareFilterState();
}

typename ParamValue<Parameter::FirmwareMode>::Type
CallibriParameterReader::readFirmwareMode() const {
    return mFirmwareMode;
}

typename ParamValue<Parameter::SamplingFrequency>::Type
CallibriParameterReader::readSamplingFrequency() const {
    return mCommonParameters->samplingFrequency();
}

typename ParamValue<Parameter::Gain>::Type
CallibriParameterReader::readGain() const {
    return mCommonParameters->gain();
}

typename ParamValue<Parameter::Offset>::Type
CallibriParameterReader::readOffset() const {
    return mCommonParameters->offset();
}

typename ParamValue<Parameter::ExternalSwitchState>::Type
CallibriParameterReader::readExternalSwitchState() const {
    return mCommonParameters->externalSwitchState();
}

typename ParamValue<Parameter::ADCInputState>::Type
CallibriParameterReader::readADCInputState() const {
    return mCommonParameters->ADCInputState();
}

typename ParamValue<Parameter::StimulatorState>::Type
CallibriParameterReader::readStimulatorState() const {
    throw std::runtime_error("Not implemented");
}

typename ParamValue<Parameter::MotionAssistantState>::Type
CallibriParameterReader::readMotionAssistantState() const {
    throw std::runtime_error("Not implemented");
}

typename ParamValue<Parameter::StimulatorParamPack>::Type
CallibriParameterReader::readStimulatorParamPack() const {
    throw std::runtime_error("Not implemented");
}

typename ParamValue<Parameter::MotionAssistantParamPack>::Type
CallibriParameterReader::readMotionAssistantParamPack() const {
    throw std::runtime_error("Not implemented");
}

bool CallibriParameterReader::loadDeviceParams(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Start init", "CallibriParameterReader", __FUNCTION__);

    if (!initAddress())
        return false;

    log->debug("[%s: %s] Address: %ld", "CallibriParameterReader", __FUNCTION__, mCommonParameters->callibriAddress());

    if (!initEcho())
        return false;

    log->debug("[%s: %s] Echo received", "CallibriParameterReader", __FUNCTION__);

    if (mFirmwareMode == FirmwareMode::Bootloader){
        log->debug("[%s: %s] Bootloader", "CallibriParameterReader", __FUNCTION__);
        activateApplication();
    }

    log->debug("[%s: %s] Requesting params", "CallibriParameterReader", __FUNCTION__);

    return mCommonParameters->syncParameters();
}

void CallibriParameterReader::sendEcho(){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::ECHO);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] ECHO received", "CallibriParameterReader", __FUNCTION__);

    if (cmdData->getError()!=ColibriCommandError::NO_ERROR) {
        throw std::runtime_error("Callibri protocol error");
    }
    auto responseLength = cmdData->getResponseLength();
    if (responseLength < 1){
        throw std::runtime_error("Callibri protocol error");
    }

    auto responseData = cmdData->getResponseData();
    mFirmwareMode = (responseData[0] & 0x80) ? FirmwareMode::Bootloader : FirmwareMode::Application;
}

void CallibriParameterReader::requestAddress(){
   mCommonParameters->setCallibriAddress(0);
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::GET_ADDR);
    log->debug("[%s: %s] Sending address request...", "CallibriParameterReader", __FUNCTION__);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError()!=ColibriCommandError::NO_ERROR) {
        log->error("[%s: %s] Failed to receive address", "CallibriParameterReader", __FUNCTION__);
        throw std::runtime_error("Callibri protocol error");
    }

    auto responseLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", "CallibriParameterReader", __FUNCTION__, responseLength);
    if (responseLength < COLIBRI_ADDRESS_LENGTH){
        throw std::runtime_error("Callibri protocol error");
    }

    auto responseData = cmdData->getResponseData();
    ByteInterpreter<unsigned long> address;
    address.value = 0;
    std::copy(responseData.begin(), responseData.begin()+COLIBRI_ADDRESS_LENGTH, address.bytes);
    log->debug("[%s: %s] Address is %ld", "CallibriParameterReader", __FUNCTION__, address.value);
    mCommonParameters->setCallibriAddress(address.value);
}

bool CallibriParameterReader::activateApplication(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::ACTIVATE_APP);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();
    log->debug("[%s: %s] Activate application response received", "CallibriParameterReader", __FUNCTION__);

    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}

bool CallibriParameterReader::initAddress(){
    auto attemptsAddrLeft = 3;
    while (attemptsAddrLeft--){
        try{
            requestAddress();
            return true;
        }
        catch (std::runtime_error &e){
            auto log = LoggerFactory::getCurrentPlatformLogger();
            log->warn("[%s: %s] Unable receive address: %s", "CallibriParameterReader", __FUNCTION__, e.what());
        }
    }
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->error("[%s: %s] Unable load device params: address", "CallibriParameterReader", __FUNCTION__);
    return false;
}

bool CallibriParameterReader::initEcho(){
    auto attemptsEchoLeft = 3;
    while(attemptsEchoLeft--){
        try {
            sendEcho();
            return true;
        }
        catch (std::runtime_error &e){
            auto log = LoggerFactory::getCurrentPlatformLogger();
            log->warn("[%s: %s] Unable receive echo response: %s", "CallibriParameterReader", __FUNCTION__, e.what());
        }
    }
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->error("[%s: %s] Unable load device params: firmware mode", "CallibriParameterReader", __FUNCTION__);
    return false;

}

}
