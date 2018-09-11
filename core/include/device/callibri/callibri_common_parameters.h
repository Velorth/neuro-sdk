#ifndef CALLIBRI_COMMON_PARAMETERS_H
#define CALLIBRI_COMMON_PARAMETERS_H

#include "device/param_values.h"
#include "device/request_scheduler.h"
#include "callibri_protocol.h"

namespace Neuro {

using CallibriRequestScheduler = RequestScheduler<CallibriCommandData>;

class CallibriCommonParameters{
public:
    CallibriCommonParameters(std::shared_ptr<CallibriRequestScheduler>);
    CallibriCommonParameters(const CallibriCommonParameters &) = delete;
    CallibriCommonParameters& operator=(const CallibriCommonParameters &) = delete;

    unsigned long serialNumber() const noexcept;
    typename ParamValue<Parameter::HardwareFilterState>::Type hardwareFilterState() const;
    typename ParamValue<Parameter::SamplingFrequency>::Type samplingFrequency() const;
    typename ParamValue<Parameter::Gain>::Type gain() const;
    typename ParamValue<Parameter::Offset>::Type offset() const;
    typename ParamValue<Parameter::ExternalSwitchState>::Type externalSwitchState() const;
    typename ParamValue<Parameter::ADCInputState>::Type ADCInputState() const;
    typename ParamValue<Parameter::AccelerometerSens>::Type accelerometerSens() const;
    typename ParamValue<Parameter::GyroscopeSens>::Type gyroscopeSens() const;
	typename ParamValue<Parameter::FirmwareVersion>::Type firmwareVersion() const;

    std::vector<CallibriModule> syncParameters();
    void setSerialNumber(unsigned long) noexcept;
    bool setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type);
    bool setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type);
    bool setGain(typename ParamValue<Parameter::Gain>::Type);
    bool setOffset(typename ParamValue<Parameter::Offset>::Type);
    bool setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type);
    bool setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type);
    bool setAccelerometerSens(typename ParamValue<Parameter::AccelerometerSens>::Type);
    bool setGyroscopeSens(typename ParamValue<Parameter::GyroscopeSens>::Type);
    std::vector<ChannelInfo> availableChannels() const;
    std::vector<Command> availableCommands() const;
    std::vector<ParamPair> availableParameters() const;
	void setFirmwareVersion(typename ParamValue<Parameter::FirmwareVersion>::Type) noexcept;

private:
    static constexpr const char *class_name = "CallibriCommonParameters";

    std::shared_ptr<CallibriRequestScheduler> mRequestHandler;
    std::vector<ChannelInfo> mAvailableChannels;
    std::vector<Command> mAvailableCommands;
    std::vector<ParamPair> mAvailableParameters;
    unsigned long mSerialNumber;
	FirmwareVersion mFirmwareVersion;

    typename ParamValue<Parameter::HardwareFilterState>::Type mHardwareFilterState;
    typename ParamValue<Parameter::SamplingFrequency>::Type mSamplingFrequency;
    typename ParamValue<Parameter::Gain>::Type mGain;
    typename ParamValue<Parameter::Offset>::Type mOffset;
    typename ParamValue<Parameter::ExternalSwitchState>::Type mExternalSwitchState;
    typename ParamValue<Parameter::ADCInputState>::Type mADCInputState;
    typename ParamValue<Parameter::AccelerometerSens>::Type mAccelerometerSens;
    typename ParamValue<Parameter::GyroscopeSens>::Type mGyroscopeSens;

    template <Parameter Param>
    bool sendSetParamCommand(typename ParamValue<Param>::Type value){
        auto cmdData = std::make_shared<CallibriCommandData>(toCallibriCommand<Param>());
        cmdData->setRequestData({byteCode(value)});
        mRequestHandler->sendRequest(cmdData);
        cmdData->wait();

        return cmdData->getError() == CallibriError::NO_ERROR;
    }
};

}

#endif // CALLIBRI_COMMON_PARAMETERS_H
