#include "device/callibri/callibri_common_parameters.h"
#include "device/callibri/callibri_protocol.h"
#include "device/request_handler.h"
#include "channels/channel_info.h"
#include "logger.h"

namespace Neuro {

CallibriCommonParameters::CallibriCommonParameters(std::shared_ptr<CallibriRequestHandler> request_handler) :
    mRequestHandler(request_handler){
}

unsigned long CallibriCommonParameters::callibriAddress() const noexcept {
    return mCallibriAddress;
}

typename ParamValue<Parameter::HardwareFilterState>::Type CallibriCommonParameters::hardwareFilterState() const {
    return mHardwareFilterState;
}

typename ParamValue<Parameter::SamplingFrequency>::Type CallibriCommonParameters::samplingFrequency() const {
    return mSamplingFrequency;
}

typename ParamValue<Parameter::Gain>::Type CallibriCommonParameters::gain() const {
    return mGain;
}

typename ParamValue<Parameter::Offset>::Type CallibriCommonParameters::offset() const {
    return mOffset;
}

typename ParamValue<Parameter::ExternalSwitchState>::Type CallibriCommonParameters::externalSwitchState() const {
    return mExternalSwitchState;
}

typename ParamValue<Parameter::ADCInputState>::Type CallibriCommonParameters::ADCInputState() const {
    return mADCInputState;
}


bool CallibriCommonParameters::setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type value){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::SWITCH_FILTER_STATE);
    std::vector<unsigned char> filterState = { static_cast<unsigned char>(value) };
    cmdData->setRequestData(filterState);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR){
        return false;
    }
    syncParameters();
    return mHardwareFilterState == value;
}


bool CallibriCommonParameters::setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type value){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::SET_FSAM);
    std::vector<unsigned char> frequencyCode = { static_cast<unsigned char>( value ) };
    cmdData->setRequestData(frequencyCode);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR){
        return false;
    }
    syncParameters();
    return mSamplingFrequency == value;
}

bool CallibriCommonParameters::setGain(typename ParamValue<Parameter::Gain>::Type value){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::SET_PGA_GAIN);
    std::vector<unsigned char> gainBytes = { static_cast<unsigned char>(value) };
    cmdData->setRequestData(gainBytes);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR){
        return false;
    }
    syncParameters();
    return mGain == value;
}

bool CallibriCommonParameters::setOffset(typename ParamValue<Parameter::Offset>::Type value){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::SET_DATA_OFFSET);
    std::vector<unsigned char> offsetBytes = { static_cast<unsigned char>(value) };
    cmdData->setRequestData(offsetBytes);
    //log->debug("[%s: %s] Sending set offset request", "CallibriCommonParameters", __FUNCTION__);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR){
        return false;
    }
    syncParameters();
    return mOffset == value;
}

bool CallibriCommonParameters::setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type value){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::SWITCH_EXT_COM_INPUTS);
    std::vector<unsigned char> adcState = { static_cast<unsigned char>(value) };
    cmdData->setRequestData(adcState);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR){
        return false;
    }
    syncParameters();
    return mExternalSwitchState == value;
}

bool CallibriCommonParameters::setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type value){
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::SWITCH_ADC_INP);
    std::vector<unsigned char> adcState = { static_cast<unsigned char>(value) };
    cmdData->setRequestData(adcState);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR){
        return false;
    }
    syncParameters();
    return mADCInputState == value;
}

std::vector<ChannelInfo> CallibriCommonParameters::availableChannels() const {
    return mAvailableChannels;
}

std::vector<Command> CallibriCommonParameters::availableCommands() const {
    return mAvailableCommands;
}

std::vector<std::pair<Parameter, ParamAccess>> CallibriCommonParameters::availableParameters() const {
    return mAvailableParameters;
}

bool CallibriCommonParameters::syncParameters(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::GET_SENSOR_PARAM);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();
    log->debug("[%s: %s] Sensor params received", "CallibriCommonParameters", __FUNCTION__);

    if (cmdData->getError() != ColibriCommandError::NO_ERROR) return false;

    auto responseDataLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", "CallibriCommonParameters", __FUNCTION__, responseDataLength);
    if (responseDataLength >= COLIBRI_SENSOR_PARAMS_DATA_LENGTH)
    {
        mAvailableChannels.clear();
        mAvailableCommands.clear();
        mAvailableParameters.clear();
        mAvailableParameters.push_back({Parameter::Name, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::State, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::Address, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::SerialNumber, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::FirmwareMode, ParamAccess::Read});
        auto responseData = cmdData->getResponseData();
        log->debug("[%s: %s] Getting features", "CallibriCommonParameters", __FUNCTION__);
        auto featuresFlags = responseData[0];
        if (featuresFlags&0x01)
        {
            mAvailableChannels.push_back(ChannelInfo::Signal);
            mAvailableChannels.push_back(ChannelInfo::ElectrodesState);
            mAvailableCommands.push_back(Command::StartSignal);
            mAvailableCommands.push_back(Command::StopSignal);

            log->debug("[%s: %s] Has signal feature", "CallibriCommonParameters", __FUNCTION__);
        }
        if (featuresFlags&0x02)
        {
            mAvailableChannels.push_back(ChannelInfo::MEMS);
            mAvailableChannels.push_back(ChannelInfo::Angle);
            mAvailableCommands.push_back(Command::StartMEMS);
            mAvailableCommands.push_back(Command::StopMEMS);
            log->debug("[%s: %s] Has MEMS feature", "CallibriCommonParameters", __FUNCTION__);
        }
        if (featuresFlags&0x04)
        {
            log->debug("[%s: %s] Has stimulation feature", "CallibriCommonParameters", __FUNCTION__);
            mAvailableCommands.push_back(Command::EnableMotionAssistant);
            mAvailableCommands.push_back(Command::StartStimulation);
            mAvailableParameters.push_back({Parameter::MotionAssistantState, ParamAccess::Read});
            mAvailableParameters.push_back({Parameter::StimulatorState, ParamAccess::Read});
            mAvailableParameters.push_back({Parameter::MotionAssistantParamPack, ParamAccess::ReadWrite});
            mAvailableParameters.push_back({Parameter::StimulatorParamPack, ParamAccess::ReadWrite});
        }
        if (featuresFlags&0x08)
        {
            mAvailableChannels.push_back(ChannelInfo::Respiration);
            mAvailableCommands.push_back(Command::StartRespiration);
            mAvailableCommands.push_back(Command::StopRespiration);
            log->debug("[%s: %s] Has spyrometry feature", "CallibriCommonParameters", __FUNCTION__);
        }        
        mAvailableChannels.push_back(ChannelInfo::ConnectionStats);
        mAvailableChannels.push_back(ChannelInfo::Battery);

        mSamplingFrequency = static_cast<SamplingFrequency>(responseData[1]);
        mAvailableParameters.push_back({Parameter::SamplingFrequency, ParamAccess::ReadWrite});
        log->debug("[%s: %s] Sampling frequency: %d Hz", "CallibriCommonParameters", __FUNCTION__, colibriSamplingFreqToInt(static_cast<ColibriSamplingFreq>(mSamplingFrequency)));

        mADCInputState = static_cast<ADCInput>(responseData[COLIBRI_ADC_INPUT_MODE_BYTE_POS]);
        mAvailableParameters.push_back({Parameter::ADCInputState, ParamAccess::ReadWrite});
        log->debug("[%s: %s] ADC input mode: %d", "CallibriCommonParameters", __FUNCTION__,
                   static_cast<int>(mADCInputState));

        mAvailableParameters.push_back({Parameter::ExternalSwitchState, ParamAccess::ReadWrite});
        mExternalSwitchState = static_cast<ExternalSwitchInput>(responseData[COLIBRI_EXT_SWITCH_STATE_BYTE_POS]);
        log->debug("[%s: %s] External switch state: %d", "CallibriCommonParameters", __FUNCTION__,
                   static_cast<int>(mExternalSwitchState));

        mAvailableParameters.push_back({Parameter::HardwareFilterState, ParamAccess::ReadWrite});
        mHardwareFilterState = responseData[COLIBRI_FILTER_STATE_BYTE_POS];
        log->debug("[%s: %s] Hign pass filter %s", "CallibriCommonParameters", __FUNCTION__, mHardwareFilterState?"enabled":"disabled");

        mAvailableParameters.push_back({Parameter::Gain, ParamAccess::ReadWrite});
        mGain = static_cast<Gain>(responseData[COLIBRI_GAIN_BYTE_POS]);
        log->debug("[%s: %s] Signal gain: %d", "CallibriCommonParameters", __FUNCTION__, colibriGainToInt(static_cast<ColibriGain>(mGain)));

        mAvailableParameters.push_back({Parameter::Offset, ParamAccess::ReadWrite});
        mOffset = responseData[COLIBRI_DATA_OFFSET_BYTE_POS];
        log->debug("[%s: %s] Data offset: %d", "CallibriCommonParameters", __FUNCTION__, mOffset);
        return true;
    }
    return false;
}

void CallibriCommonParameters::setCallibriAddress(unsigned long address) noexcept {
    mCallibriAddress = address;
}


}
