using System;
using System.Dynamic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace Neuro
{
    public enum FirmwareMode
    {
        Application,
        Bootloader
    }

    public enum DeviceState
    {
        Disconnected,
        Connected
    }

    public enum Command
    {
        StartSignal,
        StopSignal,
        StartResist,
        StopResist,
        StartMEMS,
        StopMEMS,
        StartRespiration,
        StopRespiration,
        StartStimulation,
        StopStimulation,
        EnableMotionAssistant,
        DisableMotionAssistant,
        FindMe
    }

    [StructLayout(LayoutKind.Sequential)]
    struct CommandArray
    {
        public IntPtr CmdArray;
        public UIntPtr CmdArraySize;
    }

    public enum Parameter
    {
        Name,
        State,
        Address,
        SerialNumber,
        HardwareFilterState,
        FirmwareMode,
        SamplingFrequency,
        Gain,
        Offset,
        ExternalSwitchState,
        ADCInputState,
        AccelerometerSens,
        GyroscopeSens,
        StimulatorAndMAState,
        StimulatorParamPack,
        MotionAssistantParamPack
    }

    public enum ParamAccess
    {
        Read,
        ReadWrite,
        ReadNotify
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ParamInfo
    {
        public Parameter Parameter;
        public ParamAccess Access;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct ParamInfoArray
    {
        public IntPtr InfoArray;
        public UIntPtr InfoCount;
    }

    public enum SamplingFrequency
    {
        Hz125,
        Hz250,
        Hz500,
        Hz1000,
        Hz2000,
        Hz4000,
        Hz8000
    }

    public enum Gain
    {
        Gain1,
        Gain2,
        Gain3,
        Gain4,
        Gain6,
        Gain8,
        Gain12
    }

    public enum ExternalSwitchInput
    {
        MioElectrodesRespUSB,
        MioElectrodes,
        MioUSB,
        RespUSB
    }

    public enum ADCInput
    {
        Electrodes,
        Short,
        Test,
        Resistance
    }

    public enum AccelerometerSensitivity
    {
        Sens2g,
        Sens4g,
        Sens8g,
        Sens16g
    }

    public enum GyroscopeSensitivity
    {
        Sens250Grad,
        Sens500Grad,
        Sens1000Grad,
        Sens2000Grad
    }

    public enum MotionAssistantLimb
    {
        RightLeg,
        LeftLeg,
        RightArm,
        LeftArm
    }

    public enum StimulationDeviceState
    {
        StateNoParams,
        StateDisabled,
        StateEnabled
    }

    [StructLayout(LayoutKind.Sequential)]
    struct StimulatorAndMAState
    {
        public StimulationDeviceState StimulatorState;
        public StimulationDeviceState MAState;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct MotionAssistantParams
    {
        public int GyroStart;
        public int GyroStop;
        public MotionAssistantLimb Limb;
        public int MinPause;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct StimulationParams
    {
        public int Current;
        public int PulseWidth;
        public int Frequency;
        public int StimulusDuration;
    }

    public class ParameterTypeInfo
    {
        internal delegate object ReadParamDelegate(Device device);
        internal delegate void SetParamDelegate(Device device, object value);

        public Type Type { get; }

        internal ReadParamDelegate ReadParamValue { get; }
        internal SetParamDelegate SetParamValue { get; }

        public ParameterTypeInfo(Parameter parameter)
        {
            switch (parameter)
            {
                case Parameter.Name:
                    Type = typeof(string);
                    ReadParamValue = device =>
                    {
                        var nameBuffer = new StringBuilder(128);
                        SdkError.ThrowIfError(device_read_Name(device.DevicePtr, nameBuffer, (IntPtr)nameBuffer.Capacity));
                        return nameBuffer.ToString();
                    };
                    SetParamValue = (device, value) =>
                    {
                        var name = (string) value;
                        SdkError.ThrowIfError(device_set_Name(device.DevicePtr, name));
                    };
                    break;
                case Parameter.State:
                    Type = typeof(DeviceState);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_State(device.DevicePtr, out var state));
                        return state;
                    };
                    SetParamValue = (device, value) =>
                    {
                        var state = (DeviceState)value;
                        SdkError.ThrowIfError(device_set_State(device.DevicePtr, state));
                    };
                    break;
                case Parameter.Address:
                    Type = typeof(string);
                    ReadParamValue = device =>
                    {
                        var addressBuffer = new StringBuilder(64);
                        SdkError.ThrowIfError(device_read_Address(device.DevicePtr, addressBuffer, (IntPtr)addressBuffer.Capacity));
                        return addressBuffer.ToString();
                    };
                    SetParamValue = (device, value) =>
                    {
                        var address = (string)value;
                        SdkError.ThrowIfError(device_set_Address(device.DevicePtr, address));
                    };
                    break;
                case Parameter.SerialNumber:
                    Type = typeof(string);
                    ReadParamValue = device =>
                    {
                        var stringBuilder = new StringBuilder(64);
                        SdkError.ThrowIfError(device_read_SerialNumber(device.DevicePtr, stringBuilder, (IntPtr)stringBuilder.Capacity));
                        return stringBuilder.ToString();
                    };
                    SetParamValue = (device, value) =>
                    {
                        var serial = (string)value;
                        SdkError.ThrowIfError(device_set_SerialNumber(device.DevicePtr, serial));
                    };
                    break;
                case Parameter.HardwareFilterState:
                    Type = typeof(bool);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_HardwareFilterState(device.DevicePtr, out var isEnabled));
                        return isEnabled;
                    };
                    SetParamValue = (device, value) =>
                    {
                        var filterState = (bool)value;
                        SdkError.ThrowIfError(device_set_HardwareFilterState(device.DevicePtr, filterState));
                    };
                    break;
                case Parameter.FirmwareMode:
                    Type = typeof(FirmwareMode);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_FirmwareMode(device.DevicePtr, out var firmwareMode));
                        return firmwareMode;
                    };
                    SetParamValue = (device, value) =>
                    {
                        var firmwareMode = (FirmwareMode)value;
                        SdkError.ThrowIfError(device_set_FirmwareMode(device.DevicePtr, firmwareMode));
                    };
                    break;
                case Parameter.SamplingFrequency:
                    Type = typeof(SamplingFrequency);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_SamplingFrequency(device.DevicePtr, out var samplingFrequency));
                        return samplingFrequency;
                    };
                    SetParamValue = (device, value) =>
                    {
                        var samplingFrequency = (SamplingFrequency)value;
                        SdkError.ThrowIfError(device_set_SamplingFrequency(device.DevicePtr, samplingFrequency));
                    };
                    break;
                case Parameter.Gain:
                    Type = typeof(Gain);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_Gain(device.DevicePtr, out var gain));
                        return gain;
                    };
                    SetParamValue = (device, value) =>
                    {
                        var gain = (Gain)value;
                        SdkError.ThrowIfError(device_set_Gain(device.DevicePtr, gain));
                    };
                    break;
                case Parameter.Offset:
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
                case Parameter.ExternalSwitchState:
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
                case Parameter.ADCInputState:
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
                case Parameter.AccelerometerSens:
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
                case Parameter.GyroscopeSens:
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
                case Parameter.StimulatorAndMAState:
                    Type = typeof(StimulationDeviceState);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_StimulatorAndMAState(device.DevicePtr, out var state));
                        return state;
                    };
                    SetParamValue = (device, value) => throw new InvalidOperationException("Unable to set parameter stimulation and MA state. Use appropriate command to change state");
                    break;
                case Parameter.StimulatorParamPack:
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
                case Parameter.MotionAssistantParamPack:
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
                    throw new ArgumentOutOfRangeException(nameof(parameter), parameter, null);
            }
        }

#if DEBUG
        private const string LibName = "c-neurosdkd.dll";
#else
        private const string LibName = "c-neurosdk.dll";
#endif

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Name(IntPtr devicePtr, StringBuilder outName, IntPtr bufferLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_State(IntPtr devicePtr, out DeviceState outState);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Address(IntPtr devicePtr, StringBuilder outAddress, IntPtr bufferLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_SerialNumber(IntPtr devicePtr, StringBuilder outSerial, IntPtr bufferLength);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_HardwareFilterState(IntPtr devicePtr, out bool outIsEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_FirmwareMode(IntPtr devicePtr, out FirmwareMode outMode);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_SamplingFrequency(IntPtr devicePtr, out SamplingFrequency outFreq);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Gain(IntPtr devicePtr, out Gain outGain);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_Offset(IntPtr devicePtr, out byte outOffset);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_ExternalSwitchState(IntPtr devicePtr, out ExternalSwitchInput outExtSwitch);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_ADCInputState(IntPtr devicePtr, out ADCInput outAdcInput);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_AccelerometerSens(IntPtr devicePtr, out AccelerometerSensitivity outAccelSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_GyroscopeSens(IntPtr devicePtr, out GyroscopeSensitivity outGuroSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_StimulatorAndMAState(IntPtr devicePtr, out StimulatorAndMAState outIsEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_StimulatorParamPack(IntPtr devicePtr, out StimulationParams outStimulParams);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_MotionAssistantParamPack(IntPtr devicePtr, out MotionAssistantParams outMaParams);



        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Name(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] string name);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_State(IntPtr devicePtr, DeviceState state);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Address(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] string address);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_SerialNumber(IntPtr devicePtr, [MarshalAs(UnmanagedType.LPStr)] string serial);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_HardwareFilterState(IntPtr devicePtr, bool isEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_FirmwareMode(IntPtr devicePtr, FirmwareMode mode);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_SamplingFrequency(IntPtr devicePtr, SamplingFrequency freq);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Gain(IntPtr devicePtr, Gain gain);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_Offset(IntPtr devicePtr, byte offset);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_ExternalSwitchState(IntPtr devicePtr, ExternalSwitchInput extSwitch);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_ADCInputState(IntPtr devicePtr, ADCInput adcInput);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_AccelerometerSens(IntPtr devicePtr, AccelerometerSensitivity accelSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_GyroscopeSens(IntPtr devicePtr, GyroscopeSensitivity gyroSens);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_StimulatorParamPack(IntPtr devicePtr, StimulationParams stimulParams);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_MotionAssistantParamPack(IntPtr devicePtr, MotionAssistantParams maParams);
    }
}
