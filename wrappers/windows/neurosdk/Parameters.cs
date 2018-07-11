using System;
using System.Dynamic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

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
        public uint CmdArraySize;
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

    public class ParameterTypeInfo
    {
        public Type Type { get; }

        public ParameterTypeInfo(Parameter parameter)
        {
            switch (parameter)
            {
                case Parameter.Name:
                    Type = typeof(string);
                    break;
                case Parameter.State:
                    Type = typeof(DeviceState);
                    break;
                case Parameter.Address:
                    Type = typeof(string);
                    break;
                case Parameter.SerialNumber:
                    Type = typeof(string);
                    break;
                case Parameter.HardwareFilterState:
                    Type = typeof(bool);
                    break;
                case Parameter.FirmwareMode:
                    Type = typeof(FirmwareMode);
                    break;
                case Parameter.SamplingFrequency:
                    Type = typeof(SamplingFrequency);
                    break;
                case Parameter.Gain:
                    Type = typeof(Gain);
                    break;
                case Parameter.Offset:
                    Type = typeof(byte);
                    break;
                case Parameter.ExternalSwitchState:
                    Type = typeof(ExternalSwitchInput);
                    break;
                case Parameter.ADCInputState:
                    Type = typeof(ADCInput);
                    break;
                case Parameter.AccelerometerSens:
                    Type = typeof(AccelerometerSensitivity);
                    break;
                case Parameter.GyroscopeSens:
                    Type = typeof(GyroscopeSensitivity);
                    break;
                case Parameter.StimulatorState:
                    Type = typeof(bool);
                    break;
                case Parameter.MotionAssistantState:
                    Type = typeof(bool);
                    break;
                case Parameter.StimulatorParamPack:
                    Type = typeof(StimulationParams);
                    break;
                case Parameter.MotionAssistantParamPack:
                    Type = typeof(MotionAssistantParams);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(parameter), parameter, null);
            }
        }
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
        public uint InfoCount;
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
}
