package com.neuromd.neurosdk.parameters;

import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.parameters.types.ADCInput;
import com.neuromd.neurosdk.parameters.types.AccelerometerSensitivity;
import com.neuromd.neurosdk.parameters.types.DeviceState;
import com.neuromd.neurosdk.parameters.types.ExternalSwitchInput;
import com.neuromd.neurosdk.parameters.types.FirmwareMode;
import com.neuromd.neurosdk.parameters.types.Gain;
import com.neuromd.neurosdk.parameters.types.GyroscopeSensitivity;
import com.neuromd.neurosdk.parameters.types.MotionAssistantParams;
import com.neuromd.neurosdk.parameters.types.SamplingFrequency;
import com.neuromd.neurosdk.parameters.types.StimulationParams;
import com.neuromd.neurosdk.parameters.types.StimulatorDeviceState;

final class ParameterTypeInfo {
    interface ParamReader{
        Object readParam(Device device);
    }

    interface ParamSetter{
        void setParam(Device device, Object value);
    }

    private final ParamReader mReader;
    private final ParamSetter mSetter;

    ParamReader paramReader(){
        return mReader;
    }

    ParamSetter paramSetter(){
        return mSetter;
    }

    ParameterTypeInfo(ParameterName parameter)
    {
        switch (parameter)
        {
            case Name: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadName(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        throw new UnsupportedOperationException("Unable to set device name");
                    }
                };
                break;
            }
            case State: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadState(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        throw new UnsupportedOperationException("Unable to set device state");
                    }
                };
                break;
            }
            case Address: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadAddress(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        throw new UnsupportedOperationException("Unable to set device address");
                    }
                };
                break;
            }
            case SerialNumber: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadSerialNumber(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        throw new UnsupportedOperationException("Unable to set device serial number");
                    }
                };
                break;
            }
            case HardwareFilterState: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadHardwareFilterState(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetHardwareFilterState(device.devicePtr(), (boolean) value);
                    }
                };
                break;
            }
            case FirmwareMode: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadFirmwareMode(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetFirmwareMode(device.devicePtr(), (FirmwareMode) value);
                    }
                };
                break;
            }
            case SamplingFrequency: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadSamplingFrequency(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetSamplingFrequency(device.devicePtr(), (SamplingFrequency) value);
                    }
                };
                break;
            }
            case Gain:
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadGain(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        
                    }
                };
            SetParamValue = (device, value) =>
            {
                var gain = (Gain)value;
                SdkError.ThrowIfError(device_set_Gain(device.DevicePtr, gain));
            };
            break;
            case Offset:
                Type = typeof(byte);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_Offset(device.DevicePtr, out var offset));
                return offset;
            };
            SetParamValue = (device, value) =>
            {
                var offset = (byte)value;
                SdkError.ThrowIfError(device_set_Offset(device.DevicePtr, offset));
            };
            break;
            case ExternalSwitchState:
                Type = typeof(ExternalSwitchInput);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_ExternalSwitchState(device.DevicePtr, out var externalSwitchInput));
                return externalSwitchInput;
            };
            SetParamValue = (device, value) =>
            {
                var externalSwitch = (ExternalSwitchInput)value;
                SdkError.ThrowIfError(device_set_ExternalSwitchState(device.DevicePtr, externalSwitch));
            };
            break;
            case ADCInputState:
                Type = typeof(ADCInput);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_ADCInputState(device.DevicePtr, out var adcInput));
                return adcInput;
            };
            SetParamValue = (device, value) =>
            {
                var adcInput = (ADCInput)value;
                SdkError.ThrowIfError(device_set_ADCInputState(device.DevicePtr, adcInput));
            };
            break;
            case AccelerometerSens:
                Type = typeof(AccelerometerSensitivity);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_AccelerometerSens(device.DevicePtr, out var accelerometerSensitivity));
                return accelerometerSensitivity;
            };
            SetParamValue = (device, value) =>
            {
                var accelerometerSensitivity = (AccelerometerSensitivity)value;
                SdkError.ThrowIfError(device_set_AccelerometerSens(device.DevicePtr, accelerometerSensitivity));
            };
            break;
            case GyroscopeSens:
                Type = typeof(GyroscopeSensitivity);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_GyroscopeSens(device.DevicePtr, out var gyroscopeSensitivity));
                return gyroscopeSensitivity;
            };
            SetParamValue = (device, value) =>
            {
                var gyroscopeSensitivity = (GyroscopeSensitivity)value;
                SdkError.ThrowIfError(device_set_GyroscopeSens(device.DevicePtr, gyroscopeSensitivity));
            };
            break;
            case StimulatorAndMAState:
                Type = typeof(StimulationDeviceState);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_StimulatorAndMAState(device.DevicePtr, out var state));
                return state;
            };
            SetParamValue = (device, value) => throw new InvalidOperationException("Unable to set parameter stimulation and MA state. Use appropriate command to change state");
            break;
            case StimulatorParamPack:
                Type = typeof(StimulationParams);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_StimulatorParamPack(device.DevicePtr, out var stimulationParams));
                return stimulationParams;
            };
            SetParamValue = (device, value) =>
            {
                var stimulationParams = (StimulationParams)value;
                SdkError.ThrowIfError(device_set_StimulatorParamPack(device.DevicePtr, stimulationParams));
            };
            break;
            case MotionAssistantParamPack:
                Type = typeof(MotionAssistantParams);
                ReadParamValue = device =>
            {
                SdkError.ThrowIfError(device_read_MotionAssistantParamPack(device.DevicePtr, out var motionAssistantParams));
                return motionAssistantParams;
            };
            SetParamValue = (device, value) =>
            {
                var motionAssistantParams = (MotionAssistantParams)value;
                SdkError.ThrowIfError(device_set_MotionAssistantParamPack(device.DevicePtr, motionAssistantParams));
            };
            break;
            default:
                throw new IllegalArgumentException("ParameterName");
        }
    }

        
    private static native String deviceReadName(long devicePtr);
    private static native DeviceState deviceReadState(long devicePtr);
    private static native String deviceReadAddress(long devicePtr);
    private static native String deviceReadSerialNumber(long devicePtr);
    private static native boolean deviceReadHardwareFilterState(long devicePtr);
    private static native FirmwareMode deviceReadFirmwareMode(long devicePtr);
    private static native SamplingFrequency deviceReadSamplingFrequency(long devicePtr);
    private static native Gain deviceReadGain(long devicePtr);
    private static native byte device_read_Offset(long devicePtr);
    private static native ExternalSwitchInput device_read_ExternalSwitchState(long devicePtr);
    private static native ADCInput device_read_ADCInputState(long devicePtr);
    private static native AccelerometerSensitivity device_read_AccelerometerSens(long devicePtr);
    private static native GyroscopeSensitivity device_read_GyroscopeSens(long devicePtr);
    private static native StimulatorDeviceState device_read_StimulatorAndMAState(long devicePtr);
    private static native StimulationParams device_read_StimulatorParamPack(long devicePtr);
    private static native MotionAssistantParams device_read_MotionAssistantParamPack(long devicePtr);
    private static native void deviceSetHardwareFilterState(long devicePtr, boolean isEnabled);
    private static native void deviceSetFirmwareMode(long devicePtr, FirmwareMode mode);
    private static native void deviceSetSamplingFrequency(long devicePtr, SamplingFrequency freq);
    private static native void device_set_Gain(long devicePtr, Gain gain);
    private static native void device_set_Offset(long devicePtr, byte offset);
    private static native void device_set_ExternalSwitchState(long devicePtr, ExternalSwitchInput extSwitch);
    private static native void device_set_ADCInputState(long devicePtr, ADCInput adcInput);
    private static native void device_set_AccelerometerSens(long devicePtr, AccelerometerSensitivity accelSens);
    private static native void device_set_GyroscopeSens(long devicePtr, GyroscopeSensitivity gyroSens);
    private static native void device_set_StimulatorParamPack(long devicePtr, StimulationParams stimulParams);
    private static native void device_set_MotionAssistantParamPack(long devicePtr, MotionAssistantParams maParams);
}
