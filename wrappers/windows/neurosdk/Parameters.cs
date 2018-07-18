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
        EnableMotionAssistant,
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
        StimulatorState,
        MotionAssistantState,
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

    enum MotionAssistantLimb
    {
        RightLeg,
        LeftLeg,
        RightArm,
        LeftArm
    }

    [StructLayout(LayoutKind.Sequential)]
    struct MotionAssistantParams
    {
        public int GyroStart;
        public int GyroStop;
        public MotionAssistantLimb Limb;
        public int MinPause;
        public int MaxDuration;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct StimulationParams
    {
        public int Current;
        public int PulseDuration;
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
                        SdkError.ThrowIfError(device_read_Name(device.DevicePtr, nameBuffer, (uint)nameBuffer.Capacity));
                        return nameBuffer.ToString();
                    };
                    SetParamValue = (device, value) =>
                    {

                    };
                    break;
                case Parameter.State:
                    Type = typeof(DeviceState);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_State(device.DevicePtr, out var state));
                        return state;
                    };
                    break;
                case Parameter.Address:
                    Type = typeof(string);
                    ReadParamValue = device =>
                    {
                        var addressBuffer = new StringBuilder(64);
                        SdkError.ThrowIfError(device_read_Address(device.DevicePtr, addressBuffer, (uint)addressBuffer.Capacity));
                        return addressBuffer.ToString();
                    };
                    break;
                case Parameter.SerialNumber:
                    Type = typeof(string);
                    ReadParamValue = device =>
                    {
                        var stringBuilder = new StringBuilder(64);
                        SdkError.ThrowIfError(device_read_SerialNumber(device.DevicePtr, stringBuilder, (uint)stringBuilder.Capacity));
                        return stringBuilder.ToString();
                    };
                    break;
                case Parameter.HardwareFilterState:
                    Type = typeof(bool);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_HardwareFilterState(device.DevicePtr, out var isEnabled));
                        return isEnabled;
                    };
                    break;
                case Parameter.FirmwareMode:
                    Type = typeof(FirmwareMode);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_FirmwareMode(device.DevicePtr, out var firmwareMode));
                        return firmwareMode;
                    };
                    break;
                case Parameter.SamplingFrequency:
                    Type = typeof(SamplingFrequency);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_SamplingFrequency(device.DevicePtr, out var samplingFrequency));
                        return samplingFrequency;
                    };
                    break;
                case Parameter.Gain:
                    Type = typeof(Gain);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_Gain(device.DevicePtr, out var gain));
                        return gain;
                    };
                    break;
                case Parameter.Offset:
                    Type = typeof(byte);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_Offset(device.DevicePtr, out var offset));
                        return offset;
                    };
                    break;
                case Parameter.ExternalSwitchState:
                    Type = typeof(ExternalSwitchInput);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_ExternalSwitchState(device.DevicePtr, out var externalSwitchInput));
                        return externalSwitchInput;
                    };
                    break;
                case Parameter.ADCInputState:
                    Type = typeof(ADCInput);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_ADCInputState(device.DevicePtr, out var adcInput));
                        return adcInput;
                    };
                    break;
                case Parameter.AccelerometerSens:
                    Type = typeof(AccelerometerSensitivity);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_AccelerometerSens(device.DevicePtr, out var accelerometerSensitivity));
                        return accelerometerSensitivity;
                    };
                    break;
                case Parameter.GyroscopeSens:
                    Type = typeof(GyroscopeSensitivity);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_GyroscopeSens(device.DevicePtr, out var gyroscopeSensitivity));
                        return gyroscopeSensitivity;
                    };
                    break;
                case Parameter.StimulatorState:
                    Type = typeof(bool);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_StimulatorState(device.DevicePtr, out var isEnabled));
                        return isEnabled;
                    };
                    break;
                case Parameter.MotionAssistantState:
                    Type = typeof(bool);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_MotionAssistantState(device.DevicePtr, out var isEnabled));
                        return isEnabled;
                    };
                    break;
                case Parameter.StimulatorParamPack:
                    Type = typeof(StimulationParams);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_StimulatorParamPack(device.DevicePtr, out var stimulationParams));
                        return stimulationParams;
                    };
                    break;
                case Parameter.MotionAssistantParamPack:
                    Type = typeof(MotionAssistantParams);
                    ReadParamValue = device =>
                    {
                        SdkError.ThrowIfError(device_read_MotionAssistantParamPack(device.DevicePtr, out var motionAssistantParams));
                        return motionAssistantParams;
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
        private static extern int device_read_StimulatorState(IntPtr devicePtr, out bool outIsEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_read_MotionAssistantState(IntPtr devicePtr, out bool outIsEnabled);

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
        private static extern int device_set_StimulatorState(IntPtr devicePtr, bool isEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_MotionAssistantState(IntPtr devicePtr, bool isEnabled);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_StimulatorParamPack(IntPtr devicePtr, StimulationParams stimulParams);

        [DllImport(LibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int device_set_MotionAssistantParamPack(IntPtr devicePtr, MotionAssistantParams maParams);
    }
}
