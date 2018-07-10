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

    public struct ParamInfo
    {
        public Parameter Parameter;
        public ParamAccess Access;
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
}
