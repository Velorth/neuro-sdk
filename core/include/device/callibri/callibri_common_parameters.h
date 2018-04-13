#ifndef CALLIBRI_COMMON_PARAMETERS_H
#define CALLIBRI_COMMON_PARAMETERS_H

#include "param_values.h"
#include "callibri_command.h"

namespace Neuro {

template<typename> class RequestHandler;
using CallibriRequestHandler = RequestHandler<CallibriCommandData>;

class CallibriCommonParameters{
public:
    CallibriCommonParameters(std::shared_ptr<CallibriRequestHandler>);
    CallibriCommonParameters(const CallibriCommonParameters &) = delete;
    CallibriCommonParameters& operator=(const CallibriCommonParameters &) = delete;

    unsigned long callibriAddress() const noexcept;
    typename ParamValue<Parameter::HardwareFilterState>::Type hardwareFilterState() const;
    typename ParamValue<Parameter::SamplingFrequency>::Type samplingFrequency() const;
    typename ParamValue<Parameter::Gain>::Type gain() const;
    typename ParamValue<Parameter::Offset>::Type offset() const;
    typename ParamValue<Parameter::ExternalSwitchState>::Type externalSwitchState() const;
    typename ParamValue<Parameter::ADCInputState>::Type ADCInputState() const;

    bool syncParameters();
    void setCallibriAddress(unsigned long) noexcept;
    bool setHardwareFilterState(typename ParamValue<Parameter::HardwareFilterState>::Type);
    bool setSamplingFrequency(typename ParamValue<Parameter::SamplingFrequency>::Type);
    bool setGain(typename ParamValue<Parameter::Gain>::Type);
    bool setOffset(typename ParamValue<Parameter::Offset>::Type);
    bool setExternalSwitchState(typename ParamValue<Parameter::ExternalSwitchState>::Type);
    bool setADCInputState(typename ParamValue<Parameter::ADCInputState>::Type);
    std::vector<ChannelInfo> availableChannels() const;
    std::vector<Command> availableCommands() const;
    std::vector<std::pair<Parameter, ParamAccess>> availableParameters() const;

private:
    std::shared_ptr<CallibriRequestHandler> mRequestHandler;
    std::vector<ChannelInfo> mAvailableChannels;
    std::vector<Command> mAvailableCommands;
    std::vector<std::pair<Parameter, ParamAccess>> mAvailableParameters;
    unsigned long mCallibriAddress;

    typename ParamValue<Parameter::HardwareFilterState>::Type mHardwareFilterState;
    typename ParamValue<Parameter::SamplingFrequency>::Type mSamplingFrequency;
    typename ParamValue<Parameter::Gain>::Type mGain;
    typename ParamValue<Parameter::Offset>::Type mOffset;
    typename ParamValue<Parameter::ExternalSwitchState>::Type mExternalSwitchState;
    typename ParamValue<Parameter::ADCInputState>::Type mADCInputState;
};

}

#endif // CALLIBRI_COMMON_PARAMETERS_H
