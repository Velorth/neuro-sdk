#ifndef CALLIBRI_PARAMETER_READER_H
#define CALLIBRI_PARAMETER_READER_H

#include "callibri_protocol.h"
#include "callibri_buffer_collection.h"
#include "device/parameter_reader.h"

namespace Neuro {

class CallibriCommonParameters;
template<typename> class RequestScheduler;
using CallibriRequestScheduler = RequestScheduler<CallibriCommandData>;

class CallibriParameterReader : public ParameterReader
{
public:
    CallibriParameterReader(std::shared_ptr<BleDevice>,
                            param_changed_callback_t,
                            std::shared_ptr<CallibriCommonParameters>,
                            std::shared_ptr<CallibriRequestScheduler>,
                            std::weak_ptr<CallibriBufferCollection>);

    typename ParamValue<Parameter::SerialNumber>::Type readSerialNumber() const override;
    typename ParamValue<Parameter::HardwareFilterState>::Type readHardwareFilterState() const override;
    typename ParamValue<Parameter::FirmwareMode>::Type readFirmwareMode() const override;
    typename ParamValue<Parameter::SamplingFrequency>::Type readSamplingFrequency() const override;
    typename ParamValue<Parameter::Gain>::Type readGain() const override;
    typename ParamValue<Parameter::Offset>::Type readOffset() const override;
    typename ParamValue<Parameter::ExternalSwitchState>::Type readExternalSwitchState() const override;
    typename ParamValue<Parameter::ADCInputState>::Type readADCInputState() const override;
    typename ParamValue<Parameter::AccelerometerSens>::Type readAccelerometerSens() const override;
    typename ParamValue<Parameter::GyroscopeSens>::Type readGyroscopeSens() const override;
    typename ParamValue<Parameter::StimulatorState>::Type readStimulatorState() const override;
    typename ParamValue<Parameter::MotionAssistantState>::Type readMotionAssistantState() const override;
    typename ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const override;
    typename ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const override;
	typename ParamValue<Parameter::FirmwareVersion>::Type readFirmwareVersion() const override;

private:
    static constexpr const char *class_name = "CallibriParameterReader";

    std::shared_ptr<CallibriCommonParameters> mCommonParameters;
    std::shared_ptr<CallibriRequestScheduler> mRequestHandler;
    std::weak_ptr<CallibriBufferCollection> mBufferCollection;
    FirmwareMode mFirmwareMode;

    bool loadDeviceParams() override;
    void sendEcho();
    void requestSerialNumber();
    bool activateApplication();
    bool initAddress();
    bool initEcho();
    void createBuffers(std::vector<CallibriModule>);
};

}
#endif // CALLIBRI_PARAMETER_READER_H
