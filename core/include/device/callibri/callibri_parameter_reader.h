#ifndef CALLIBRI_PARAMETER_READER_H
#define CALLIBRI_PARAMETER_READER_H

#include "device/parameter_reader.h"
#include "callibri_command.h"

namespace Neuro {

class CallibriCommonParameters;
template<typename> class RequestHandler;
using CallibriRequestHandler = RequestHandler<CallibriCommandData>;

class CallibriParameterReader : public ParameterReader
{
public:
    CallibriParameterReader(std::shared_ptr<BleDevice>,
                            param_changed_callback_t,
                            std::shared_ptr<CallibriCommonParameters>,
                            std::shared_ptr<CallibriRequestHandler>);

    typename ParamValue<Parameter::SerialNumber>::Type readSerialNumber() const override;
    typename ParamValue<Parameter::HardwareFilterState>::Type readHardwareFilterState() const override;
    typename ParamValue<Parameter::FirmwareMode>::Type readFirmwareMode() const override;
    typename ParamValue<Parameter::SamplingFrequency>::Type readSamplingFrequency() const override;
    typename ParamValue<Parameter::Gain>::Type readGain() const override;
    typename ParamValue<Parameter::Offset>::Type readOffset() const override;
    typename ParamValue<Parameter::ExternalSwitchState>::Type readExternalSwitchState() const override;
    typename ParamValue<Parameter::ADCInputState>::Type readADCInputState() const override;
    typename ParamValue<Parameter::StimulatorState>::Type readStimulatorState() const override;
    typename ParamValue<Parameter::MotionAssistantState>::Type readMotionAssistantState() const override;
    typename ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const override;
    typename ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const override;

private:
    std::shared_ptr<CallibriCommonParameters> mCommonParameters;
    std::shared_ptr<CallibriRequestHandler> mRequestHandler;
    FirmwareMode mFirmwareMode;

    bool loadDeviceParams() override;
    void sendEcho();
    void requestSerialNumber();
    bool activateApplication();
    bool initAddress();
    bool initEcho();
};

}
#endif // CALLIBRI_PARAMETER_READER_H
