#ifndef PARAMETER_READER_H
#define PARAMETER_READER_H

#include <memory>
#include "ble_device.h"
#include "param_values.h"
#include "event_observer.h"

namespace Neuro {

class ParameterReader{
public:
    ParameterReader(std::shared_ptr<BleDevice>);
    virtual ~ParameterReader() = default;

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
    typename ParamValue<Parameter::StimulatorState>::Type readStimulatorState() const = 0;
    virtual
    typename ParamValue<Parameter::MotionAssistantState>::Type readMotionAssistantState() const = 0;
    virtual
    typename ParamValue<Parameter::StimulatorParamPack>::Type readStimulatorParamPack() const = 0;
    virtual
    typename ParamValue<Parameter::MotionAssistantParamPack>::Type readMotionAssistantParamPack() const = 0;

private:
    std::shared_ptr<BleDevice> mBleDevice;
    DeviceState mState{DeviceState::Disconnected};

    void subscribeBleDeviceStateChanged();
    void onBleDeviceStateChanged(BleDeviceState, BleDeviceError);
    void onBleConnected();
    void onBleDisconnected(BleDeviceError);
    virtual bool loadDeviceParams() = 0;

    EventHandler(BleDevice, deviceStateChanged) bleDeviceStateHandler;
};

}

#endif // PARAMETER_READER_H
