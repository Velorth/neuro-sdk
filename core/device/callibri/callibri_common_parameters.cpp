#include "device/callibri/callibri_common_parameters.h"
#include "channels/channel_info.h"
#include "logger.h"

namespace Neuro {

CallibriCommonParameters::CallibriCommonParameters(std::shared_ptr<CallibriRequestScheduler> request_handler) :
    mRequestHandler(request_handler){
}

unsigned long CallibriCommonParameters::serialNumber() const noexcept {
    return mSerialNumber;
}

typename ParamValue<Parameter::HardwareFilterState>::Type
CallibriCommonParameters::hardwareFilterState() const {
    return mHardwareFilterState;
}

typename ParamValue<Parameter::SamplingFrequency>::Type
CallibriCommonParameters::samplingFrequency() const {
    return mSamplingFrequency;
}

typename ParamValue<Parameter::Gain>::Type
CallibriCommonParameters::gain() const {
    return mGain;
}

typename ParamValue<Parameter::Offset>::Type
CallibriCommonParameters::offset() const {
    return mOffset;
}

typename ParamValue<Parameter::ExternalSwitchState>::Type
CallibriCommonParameters::externalSwitchState() const {
    return mExternalSwitchState;
}

typename ParamValue<Parameter::ADCInputState>::Type
CallibriCommonParameters::ADCInputState() const {
    return mADCInputState;
}

typename ParamValue<Parameter::AccelerometerSens>::Type
CallibriCommonParameters::accelerometerSens() const{
    return mAccelerometerSens;
}

typename ParamValue<Parameter::GyroscopeSens>::Type
CallibriCommonParameters::gyroscopeSens() const {
    return mGyroscopeSens;
}


bool CallibriCommonParameters::setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type value){
    if (!sendSetParamCommand<Parameter::HardwareFilterState>(value))
        return false;
    syncParameters();
    return mHardwareFilterState == value;
}


bool CallibriCommonParameters::setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type value){
    if (!sendSetParamCommand<Parameter::SamplingFrequency>(value))
        return false;
    syncParameters();
    return mSamplingFrequency == value;
}

bool CallibriCommonParameters::setGain(typename ParamValue<Parameter::Gain>::Type value){
    if (!sendSetParamCommand<Parameter::Gain>(value))
        return false;
    syncParameters();
    return mGain == value;
}

bool CallibriCommonParameters::setOffset(typename ParamValue<Parameter::Offset>::Type value){
    if (!sendSetParamCommand<Parameter::Offset>(value))
        return false;
    syncParameters();
    return mOffset == value;
}

bool CallibriCommonParameters::setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type value){
    if (!sendSetParamCommand<Parameter::ExternalSwitchState>(value))
        return false;
    syncParameters();
    return mExternalSwitchState == value;
}

bool CallibriCommonParameters::setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type value){
    if (!sendSetParamCommand<Parameter::ADCInputState>(value))
        return false;
    syncParameters();
    return mADCInputState == value;
}

bool CallibriCommonParameters::setAccelerometerSens(typename ParamValue<Parameter::AccelerometerSens>::Type value){
    if (!sendSetParamCommand<Parameter::AccelerometerSens>(value))
        return false;
    syncParameters();
    return mAccelerometerSens == value;
}

bool CallibriCommonParameters::setGyroscopeSens(typename ParamValue<Parameter::GyroscopeSens>::Type value){
    if (!sendSetParamCommand<Parameter::GyroscopeSens>(value))
        return false;
    syncParameters();
    return mGyroscopeSens == value;
}

std::vector<ChannelInfo> CallibriCommonParameters::availableChannels() const {
    return mAvailableChannels;
}

std::vector<Command> CallibriCommonParameters::availableCommands() const {
    return mAvailableCommands;
}

std::vector<ParamPair> CallibriCommonParameters::availableParameters() const {
    return mAvailableParameters;
}

bool CallibriCommonParameters::syncParameters(){
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::GET_SENSOR_PARAM);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();
    LOG_DEBUG("Sensor params received");

    if (cmdData->getError() != CallibriError::NO_ERROR)
        return false;

    auto responseDataLength = cmdData->getResponseLength();
    LOG_DEBUG_V("Response length: %zd", responseDataLength);
    if (responseDataLength >= CallibriParamsDataLength){
        mAvailableChannels.clear();
        mAvailableCommands.clear();
        mAvailableParameters.clear();
        mAvailableParameters.push_back({Parameter::Name, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::State, ParamAccess::ReadNotify});
        mAvailableParameters.push_back({Parameter::Address, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::SerialNumber, ParamAccess::Read});
        mAvailableParameters.push_back({Parameter::FirmwareMode, ParamAccess::Read});

        LOG_DEBUG("Getting features");
        auto responseData = cmdData->getResponseData();
        auto featuresFlags = responseData[0];
        if (featuresFlags&0x01){
            mAvailableChannels.push_back(ChannelInfo::Signal);
            mAvailableChannels.push_back(ChannelInfo::ElectrodesState);
            mAvailableCommands.push_back(Command::StartSignal);
            mAvailableCommands.push_back(Command::StopSignal);
            LOG_DEBUG("Has signal feature");
        }
        if (featuresFlags&0x02){
            mAvailableChannels.push_back(ChannelInfo::MEMS);
            mAvailableChannels.push_back(ChannelInfo::Angle);
            mAvailableCommands.push_back(Command::StartMEMS);
            mAvailableCommands.push_back(Command::StopMEMS);
            LOG_DEBUG("Has MEMS feature");
        }
        if (featuresFlags&0x04){
            mAvailableCommands.push_back(Command::EnableMotionAssistant);
            mAvailableCommands.push_back(Command::StartStimulation);
            mAvailableParameters.push_back({Parameter::MotionAssistantState, ParamAccess::Read});
            mAvailableParameters.push_back({Parameter::StimulatorState, ParamAccess::Read});
            mAvailableParameters.push_back({Parameter::MotionAssistantParamPack, ParamAccess::ReadWrite});
            mAvailableParameters.push_back({Parameter::StimulatorParamPack, ParamAccess::ReadWrite});            
            LOG_DEBUG("Has stimulation feature");
        }
        if (featuresFlags&0x08){
            mAvailableChannels.push_back(ChannelInfo::Respiration);
            mAvailableCommands.push_back(Command::StartRespiration);
            mAvailableCommands.push_back(Command::StopRespiration);
            LOG_DEBUG("Has spyrometry feature");
        }

        mAvailableChannels.push_back(ChannelInfo::ConnectionStats);
        mAvailableChannels.push_back(ChannelInfo::Battery);

        mSamplingFrequency = fromByteCode<SamplingFrequency>(responseData[CallibriSamplFreqBytePos]);
        mAvailableParameters.push_back({Parameter::SamplingFrequency, ParamAccess::ReadWrite});
        LOG_DEBUG_V("Sampling frequency: %d Hz", intValue(mSamplingFrequency));

        mADCInputState = fromByteCode<ADCInput>(responseData[CallibriAdcInputBytePos]);
        mAvailableParameters.push_back({Parameter::ADCInputState, ParamAccess::ReadWrite});
        LOG_DEBUG_V("ADC input mode: %d", static_cast<int>(mADCInputState));

        mExternalSwitchState = fromByteCode<ExternalSwitchInput>(responseData[CallibriExtSwithBytePos]);
        mAvailableParameters.push_back({Parameter::ExternalSwitchState, ParamAccess::ReadWrite});
        LOG_DEBUG_V("External switch state: %d", static_cast<int>(mExternalSwitchState));

        mHardwareFilterState = static_cast<bool>(responseData[CallibriFilterStateBytePos]);
        mAvailableParameters.push_back({Parameter::HardwareFilterState, ParamAccess::ReadWrite});
        LOG_DEBUG_V("Hign pass filter %s", mHardwareFilterState?"enabled":"disabled");


        mGain = fromByteCode<Gain>(responseData[CallibriGainBytePos]);
        mAvailableParameters.push_back({Parameter::Gain, ParamAccess::ReadWrite});
        LOG_DEBUG_V("Signal gain: %d", intValue(mGain));

        mOffset = responseData[CallibriOffsetBytePos];
        mAvailableParameters.push_back({Parameter::Offset, ParamAccess::ReadWrite});
        LOG_DEBUG_V("Data offset: %d", mOffset);

        mAccelerometerSens = fromByteCode<AccelerometerSensitivity>(responseData[CallibriAccelSensBytePos]);
        mAvailableParameters.push_back({Parameter::AccelerometerSens, ParamAccess::ReadWrite});
        LOG_DEBUG_V("Accelerometer sens: %d", responseData[CallibriAccelSensBytePos]);

        mGyroscopeSens = fromByteCode<GyroscopeSensitivity>(responseData[CallibriGyroSensBytePos]);
        mAvailableParameters.push_back({Parameter::GyroscopeSens, ParamAccess::ReadWrite});
        LOG_DEBUG_V("Gyroscope sens: %d", responseData[CallibriGyroSensBytePos]);

        return true;
    }
    return false;
}

void CallibriCommonParameters::setSerialNumber(unsigned long address) noexcept {
    mSerialNumber = address;
}


}
