#ifndef PARAMETER_READER_H
#define PARAMETER_READER_H

#include <functional>
#include <memory>
#include "ble/ble_device.h"
#include "param_values.h"

namespace Neuro {

class ParameterReader{
public:
    using param_changed_callback_t = std::function<void(Parameter)>;

    ParameterReader(std::shared_ptr<BleDevice>, param_changed_callback_t);
    virtual ~ParameterReader() = default;

    void requestConnect();
    void requestDisconnect();
    typename ParamValue<Parameter::Name>::Type readName() const;
    typename ParamValue<Parameter::State>::Type readState() const;
    typename ParamValue<Parameter::Address>::Type readAddress() const;
    virtual
    typename ParamValue<Parameter::SerialNumber>::Type readSerialNumber() const = 0;
    virtual
    typename ParamValue<Parameter::HardwareFilterState>::Type readHardwareFilterState() const = 0;
    virtual
    typename ParamValue<Parameter::FirmwareMode>::Type readFirmwareMode() const = 0;
    virtual
    typename ParamValue<Parameter::SamplingFrequency>::Type readSamplingFrequency() const = 0;
    virtual
    typename ParamValue<Parameter::Gain>::Type readGain() const = 0;
    virtual
    typename ParamValue<Parameter::Offset>::Type readOffset() const = 0;
    virtual
    typename ParamValue<Parameter::ExternalSwitchState>::Type readExternalSwitchState() const = 0;
    virtual
    typename ParamValue<Parameter::ADCInputState>::Type readADCInputState() const = 0;
    virtual
    typename ParamValue<Parameter::AccelerometerSens>::Type readAccelerometerSens() const = 0;
    virtual
    typename ParamValue<Parameter::GyroscopeSens>::Type readGyroscopeSens() const = 0;
    virtual
    typename ParamValue<Parameter::StimulatorAndMAState>::Type readStimulatorAndMAState() const = 0;
    virtual
    typename ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const = 0;
    virtual
    typename ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const = 0;
	virtual
	typename ParamValue<Parameter::FirmwareVersion>::Type readFirmwareVersion() const = 0;

private:
    static constexpr const char *class_name = "ParameterReader";

    std::shared_ptr<BleDevice> mBleDevice;
    DeviceState mState{DeviceState::Disconnected};
    bool mPendingDisconnectRequest{false};
    param_changed_callback_t parameterChangedCallback;

    void subscribeBleDeviceStateChanged();
    void onBleDeviceStateChanged(BleDeviceState, BleDeviceError);
    void onBleConnected();
    void onBleDisconnected(BleDeviceError);
    virtual bool loadDeviceParams() = 0;
};

}

#endif // PARAMETER_READER_H
