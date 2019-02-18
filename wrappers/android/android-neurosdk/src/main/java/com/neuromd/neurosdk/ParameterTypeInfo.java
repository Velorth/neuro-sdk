package com.neuromd.neurosdk;

import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.parameters.ParameterName;
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
            case Gain: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadGain(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetGain(device.devicePtr(), (Gain) value);
                    }
                };
                break;
            }
            case Offset: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadOffset(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetOffset(device.devicePtr(), (byte) value);
                    }
                };
                break;
            }
            case ExternalSwitchState: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadExternalSwitchState(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetExternalSwitchState(device.devicePtr(), (ExternalSwitchInput) value);
                    }
                };
                break;
            }
            case ADCInputState: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadADCInputState(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetADCInputState(device.devicePtr(), (ADCInput) value);
                    }
                };
                break;
            }
            case AccelerometerSens: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadAccelerometerSens(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetAccelerometerSens(device.devicePtr(), (AccelerometerSensitivity) value);
                    }
                };
                break;
            }
            case GyroscopeSens: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadGyroscopeSens(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetGyroscopeSens(device.devicePtr(), (GyroscopeSensitivity) value);
                    }
                };
                break;
            }
            case StimulatorAndMAState: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadStimulatorAndMAState(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        throw new UnsupportedOperationException("Unable to set stimulation state");
                    }
                };
                break;
            }
            case StimulatorParamPack:
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadStimulatorParamPack(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetStimulatorParamPack(device.devicePtr(), (StimulationParams)value);
                    }
                };
            break;
            case MotionAssistantParamPack: {
                mReader = new ParamReader() {
                    @Override
                    public Object readParam(Device device) {
                        return deviceReadMotionAssistantParamPack(device.devicePtr());
                    }
                };
                mSetter = new ParamSetter() {
                    @Override
                    public void setParam(Device device, Object value) {
                        deviceSetMotionAssistantParamPack(device.devicePtr(), (MotionAssistantParams) value);
                    }
                };
                break;
            }
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
    private static native byte deviceReadOffset(long devicePtr);
    private static native ExternalSwitchInput deviceReadExternalSwitchState(long devicePtr);
    private static native ADCInput deviceReadADCInputState(long devicePtr);
    private static native AccelerometerSensitivity deviceReadAccelerometerSens(long devicePtr);
    private static native GyroscopeSensitivity deviceReadGyroscopeSens(long devicePtr);
    private static native StimulatorDeviceState deviceReadStimulatorAndMAState(long devicePtr);
    private static native StimulationParams deviceReadStimulatorParamPack(long devicePtr);
    private static native MotionAssistantParams deviceReadMotionAssistantParamPack(long devicePtr);
    private static native void deviceSetHardwareFilterState(long devicePtr, boolean isEnabled);
    private static native void deviceSetFirmwareMode(long devicePtr, FirmwareMode mode);
    private static native void deviceSetSamplingFrequency(long devicePtr, SamplingFrequency freq);
    private static native void deviceSetGain(long devicePtr, Gain gain);
    private static native void deviceSetOffset(long devicePtr, byte offset);
    private static native void deviceSetExternalSwitchState(long devicePtr, ExternalSwitchInput extSwitch);
    private static native void deviceSetADCInputState(long devicePtr, ADCInput adcInput);
    private static native void deviceSetAccelerometerSens(long devicePtr, AccelerometerSensitivity accelSens);
    private static native void deviceSetGyroscopeSens(long devicePtr, GyroscopeSensitivity gyroSens);
    private static native void deviceSetStimulatorParamPack(long devicePtr, StimulationParams stimulParams);
    private static native void deviceSetMotionAssistantParamPack(long devicePtr, MotionAssistantParams maParams);
}
