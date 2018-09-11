#include "device/callibri/callibri_parameter_reader.h"
#include "device/callibri/callibri_common_parameters.h"
#include "device/callibri/callibri_command.h"
#include "device/callibri/callibri_protocol.h"
#include "device/request_scheduler.h"
#include "common_types.h"

namespace Neuro {

CallibriParameterReader::CallibriParameterReader(std::shared_ptr<BleDevice> ble_device,
                                                 param_changed_callback_t callback,
                                                 std::shared_ptr<CallibriCommonParameters> common_params,
                                                 std::shared_ptr<CallibriRequestScheduler> request_handler,
                                                 std::weak_ptr<CallibriBufferCollection> buffer_collection) :
    ParameterReader(ble_device, callback),
    mCommonParameters(common_params),
    mRequestHandler(request_handler),
    mBufferCollection(buffer_collection){

}

typename ParamValue<Parameter::SerialNumber>::Type
CallibriParameterReader::readSerialNumber() const {
    return std::to_string(mCommonParameters->serialNumber());
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

typename ParamValue<Parameter::AccelerometerSens>::Type
CallibriParameterReader::readAccelerometerSens() const {
    return mCommonParameters->accelerometerSens();
}

typename ParamValue<Parameter::GyroscopeSens>::Type
CallibriParameterReader::readGyroscopeSens() const {
    return mCommonParameters->gyroscopeSens();
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

typename ParamValue<Parameter::FirmwareVersion>::Type
CallibriParameterReader::readFirmwareVersion() const {
	return mCommonParameters->firmwareVersion();
}

bool CallibriParameterReader::loadDeviceParams(){
    LOG_DEBUG("Start init");
    if (!initAddress())
        return false;

    LOG_DEBUG_V("Address: %ld", mCommonParameters->serialNumber());
    if (!initEcho())
        return false;

    LOG_DEBUG("Echo received");
    if (mFirmwareMode == FirmwareMode::Bootloader){
        LOG_DEBUG("Bootloader");
        activateApplication();
    }
    LOG_DEBUG("Requesting params");
    try {
        auto modules = mCommonParameters->syncParameters();
        createBuffers(modules);
        return true;
    }
    catch(std::runtime_error &e){
        LOG_ERROR_V("Unable sync device params: %s", e.what());
        return false;
    }
}

void CallibriParameterReader::sendEcho(){
	mCommonParameters->setFirmwareVersion(FirmwareVersion{0,0});
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::ECHO);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    LOG_DEBUG("ECHO received");

    if (cmdData->getError()!=CallibriError::NO_ERROR) {
        throw std::runtime_error("Callibri protocol error");
    }
    auto responseLength = cmdData->getResponseLength();
    if (responseLength < 1){
        throw std::runtime_error("Callibri protocol error");
    }

    auto responseData = cmdData->getResponseData();
    mFirmwareMode = (responseData[0] & 0x80) ? FirmwareMode::Bootloader : FirmwareMode::Application;
	FirmwareVersion version{};
	version.Version = responseData[0] & 0x3F;
	ByteInterpreter<unsigned short> buildInerpreter;
	buildInerpreter.value = 0;
	buildInerpreter.bytes[0] = responseData[1];
	buildInerpreter.bytes[1] = responseData[2];
	version.Build = buildInerpreter.value;
	mCommonParameters->setFirmwareVersion(version);
}

void CallibriParameterReader::requestSerialNumber(){
    mCommonParameters->setSerialNumber(0);
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::GET_ADDR);
    LOG_DEBUG("Sending address request...");
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError()!=CallibriError::NO_ERROR) {
        LOG_ERROR("Failed to receive address");
        throw std::runtime_error("Callibri protocol error");
    }

    auto responseLength = cmdData->getResponseLength();
    LOG_DEBUG_V("Response length: %zd", responseLength);
    if (responseLength < CallibriAddressLength){
        throw std::runtime_error("Callibri protocol error");
    }

    auto responseData = cmdData->getResponseData();
    ByteInterpreter<unsigned long> address;
    address.value = 0;
    std::copy(responseData.begin(), responseData.begin()+CallibriAddressLength, address.bytes);
    LOG_DEBUG_V("Address is %ld", address.value);
    mCommonParameters->setSerialNumber(address.value);
}

bool CallibriParameterReader::activateApplication(){

    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::ACTIVATE_APP);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();
    LOG_DEBUG("Activate application response received");

    return cmdData->getError() == CallibriError::NO_ERROR;
}

bool CallibriParameterReader::initAddress(){
    auto attemptsAddrLeft = 3;
    while (attemptsAddrLeft--){
        try{
            requestSerialNumber();
            return true;
        }
        catch (std::runtime_error &e){
            LOG_WARN_V("Unable receive address: %s", e.what());
        }
    }

    LOG_ERROR("Unable load device params: serial number");
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
            LOG_WARN_V("Unable receive echo response: %s", e.what());
        }
    }

    LOG_ERROR("Unable load device params: firmware mode");
    return false;

}

void CallibriParameterReader::createBuffers(std::vector<CallibriModule> modules){
    auto bufferCollectionPtr = mBufferCollection.lock();
    if (!bufferCollectionPtr){
        throw std::runtime_error("Unable create buffers. Collection ptr is null");
    }

    if (std::find(modules.begin(), modules.end(), CallibriModule::Signal) != modules.end()
            && !bufferCollectionPtr->hasSignalBuffer()){
        auto signalBuffer = std::make_unique<CallibriSignalBuffer>(mCommonParameters);
        bufferCollectionPtr->setSignalBuffer(std::move(signalBuffer));
    }

    if (std::find(modules.begin(), modules.end(), CallibriModule::Respiration) != modules.end()
            && !bufferCollectionPtr->hasRespirationBuffer()){
        auto respBuffer = std::make_unique<CallibriRespirationBuffer>();
        bufferCollectionPtr->setRespirationBuffer(std::move(respBuffer));
    }

    if (std::find(modules.begin(), modules.end(), CallibriModule::MEMS) != modules.end()){
        if (!bufferCollectionPtr->hasMemsBuffer()){
            auto memsBuffer = std::make_unique<CallibriMemsBuffer>(mCommonParameters);
            bufferCollectionPtr->setMemsBuffer(std::move(memsBuffer));
        }
        if (!bufferCollectionPtr->hasOrientationBuffer()){
            auto angleBuffer = std::make_unique<CallibriOrientationBuffer>();
            bufferCollectionPtr->setOrientationBuffer(std::move(angleBuffer));
        }
    }
}

}
