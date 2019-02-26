#include "java_helper.h"

template<>
const std::unordered_map<ADCInput, std::string>
        enum_name_map<ADCInput>::mEnumToNameMap = []() {
    return std::unordered_map<ADCInput, std::string>{
            {ADCInputResistance, "Resistance"},
            {ADCInputElectrodes, "Electrodes"},
            {ADCInputShort,      "Short"},
            {ADCInputTest,       "Test"}
    };
}();

template<>
const std::unordered_map<std::string, ADCInput>
        enum_name_map<ADCInput>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, ADCInput>{
            {"Resistance", ADCInputResistance},
            {"Electrodes", ADCInputElectrodes},
            {"Short",      ADCInputShort},
            {"Test",       ADCInputTest}
    };
}();

template<>
const std::unordered_map<AccelerometerSensitivity, std::string>
        enum_name_map<AccelerometerSensitivity>::mEnumToNameMap = []() {
    return std::unordered_map<AccelerometerSensitivity, std::string>{
            {AccelerometerSens2g,  "Sens2g"},
            {AccelerometerSens4g,  "Sens4g"},
            {AccelerometerSens8g,  "Sens8g"},
            {AccelerometerSens16g, "Sens16g"}
    };
}();

template<>
const std::unordered_map<std::string, AccelerometerSensitivity>
        enum_name_map<AccelerometerSensitivity>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, AccelerometerSensitivity>{
            {"Sens2g",  AccelerometerSens2g},
            {"Sens4g",  AccelerometerSens4g},
            {"Sens8g",  AccelerometerSens8g},
            {"Sens16g", AccelerometerSens16g}
    };
}();

template<>
const std::unordered_map<GyroscopeSensitivity, std::string>
        enum_name_map<GyroscopeSensitivity>::mEnumToNameMap = []() {
    return std::unordered_map<GyroscopeSensitivity, std::string>{
            {GyroscopeSens250Grad,  "Sens250Grad"},
            {GyroscopeSens500Grad,  "Sens500Grad"},
            {GyroscopeSens1000Grad, "Sens1000Grad"},
            {GyroscopeSens2000Grad, "Sens2000Grad"}
    };
}();

template<>
const std::unordered_map<std::string, GyroscopeSensitivity>
        enum_name_map<GyroscopeSensitivity>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, GyroscopeSensitivity>{
            {"Sens250Grad",  GyroscopeSens250Grad},
            {"Sens500Grad",  GyroscopeSens500Grad},
            {"Sens1000Grad", GyroscopeSens1000Grad},
            {"Sens2000Grad", GyroscopeSens2000Grad}
    };
}();

template<>
const std::unordered_map<ExternalSwitchInput, std::string>
        enum_name_map<ExternalSwitchInput>::mEnumToNameMap = []() {
    return std::unordered_map<ExternalSwitchInput, std::string>{
            {ExternalSwitchInputMioElectrodesRespUSB, "MioElectrodesRespUSB"},
            {ExternalSwitchInputMioElectrodes,        "MioElectrodes"},
            {ExternalSwitchInputMioUSB,               "MioUSB"},
            {ExternalSwitchInputRespUSB,              "RespUSB"}
    };
}();

template<>
const std::unordered_map<std::string, ExternalSwitchInput>
        enum_name_map<ExternalSwitchInput>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, ExternalSwitchInput>{
            {"MioElectrodesRespUSB", ExternalSwitchInputMioElectrodesRespUSB},
            {"MioElectrodes",        ExternalSwitchInputMioElectrodes},
            {"MioUSB",               ExternalSwitchInputMioUSB},
            {"RespUSB",              ExternalSwitchInputRespUSB}
    };
}();

template<>
const std::unordered_map<FirmwareMode, std::string>
        enum_name_map<FirmwareMode>::mEnumToNameMap = []() {
    return std::unordered_map<FirmwareMode, std::string>{
            {FirmwareModeApplication, "Application"},
            {FirmwareModeBootloader,  "Bootloader"}
    };
}();

template<>
const std::unordered_map<std::string, FirmwareMode>
        enum_name_map<FirmwareMode>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, FirmwareMode>{
            {"Application", FirmwareModeApplication},
            {"Bootloader",  FirmwareModeBootloader}
    };
}();

template<>
const std::unordered_map<Gain, std::string>
        enum_name_map<Gain>::mEnumToNameMap = []() {
    return std::unordered_map<Gain, std::string>{
            {Gain1, "Gain1"},
            {Gain2, "Gain2"},
            {Gain3, "Gain3"},
            {Gain4, "Gain4"},
            {Gain6, "Gain6"},
            {Gain8, "Gain8"},
            {Gain12, "Gain12"}
    };
}();

template<>
const std::unordered_map<std::string, Gain>
        enum_name_map<Gain>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, Gain>{
            {"Gain1", Gain1},
            {"Gain2", Gain2},
            {"Gain3", Gain3},
            {"Gain4", Gain4},
            {"Gain6", Gain6},
            {"Gain8", Gain8},
            {"Gain12", Gain12}
    };
}();

template<>
const std::unordered_map<MotionAssistantLimb, std::string>
        enum_name_map<MotionAssistantLimb>::mEnumToNameMap = []() {
    return std::unordered_map<MotionAssistantLimb, std::string>{
            {MotionAssistantLimbRightLeg, "RightLeg"},
            {MotionAssistantLimbLeftLeg,  "LeftLeg"},
            {MotionAssistantLimbRightArm, "RightArm"},
            {MotionAssistantLimbLeftArm,  "LeftArm"}
    };
}();

template<>
const std::unordered_map<std::string, MotionAssistantLimb>
        enum_name_map<MotionAssistantLimb>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, MotionAssistantLimb>{
            {"RightLeg", MotionAssistantLimbRightLeg},
            {"LeftLeg", MotionAssistantLimbLeftLeg},
            {"RightArm", MotionAssistantLimbRightArm},
            {"LeftArm", MotionAssistantLimbLeftArm}
    };
}();


template<>
const std::unordered_map<StimulationDeviceState, std::string>
        enum_name_map<StimulationDeviceState>::mEnumToNameMap = []() {
    return std::unordered_map<StimulationDeviceState, std::string>{
            {StateNoParams , "NoParams"},
            {StateDisabled,  "Disabled"},
            {StateEnabled, "Enabled"}
    };
}();

template<>
const std::unordered_map<std::string, StimulationDeviceState>
        enum_name_map<StimulationDeviceState>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, StimulationDeviceState>{
            {"NoParams", StateNoParams},
            {"Disabled", StateDisabled},
            {"Enabled", StateEnabled}
    };
}();

template<>
const std::unordered_map<SamplingFrequency, std::string>
        enum_name_map<SamplingFrequency>::mEnumToNameMap = []() {
    return std::unordered_map<SamplingFrequency, std::string>{
            {SamplingFrequencyHz125,  "Hz125"},
            {SamplingFrequencyHz250,  "Hz250"},
            {SamplingFrequencyHz500,  "Hz500"},
            {SamplingFrequencyHz1000, "Hz1000"},
            {SamplingFrequencyHz2000, "Hz2000"},
            {SamplingFrequencyHz4000, "Hz4000"},
            {SamplingFrequencyHz8000, "Hz8000"}
    };
}();

template<>
const std::unordered_map<std::string, SamplingFrequency>
        enum_name_map<SamplingFrequency>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, SamplingFrequency>{
            {"Hz125", SamplingFrequencyHz125},
            {"Hz250", SamplingFrequencyHz250},
            {"Hz500", SamplingFrequencyHz500},
            {"Hz1000", SamplingFrequencyHz1000},
            {"Hz2000", SamplingFrequencyHz2000},
            {"Hz4000", SamplingFrequencyHz4000},
            {"Hz8000", SamplingFrequencyHz8000}
    };
}();

template<>
const std::unordered_map<ChannelType, std::string>
        enum_name_map<ChannelType>::mEnumToNameMap = []() {
    return std::unordered_map<ChannelType, std::string>{
            {ChannelTypeSignal,          "Signal"},
            {ChannelTypeBattery,         "Battery"},
            {ChannelTypeElectrodesState, "ElectrodesState"},
            {ChannelTypeRespiration,     "Respiration"},
            {ChannelTypeMEMS,            "MEMS"},
            {ChannelTypeOrientation,     "Orientation"},
            {ChannelTypeConnectionStats, "ConnectionStats"},
            {ChannelTypeResistance,      "Resistance"},
            {ChannelTypePedometer,       "Pedometer"},
            {ChannelTypeCustom,          "Custom"}
    };
}();

template<>
const std::unordered_map<std::string, ChannelType>
        enum_name_map<ChannelType>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, ChannelType>{
            {"Signal",          ChannelTypeSignal},
            {"Battery",         ChannelTypeBattery},
            {"ElectrodesState", ChannelTypeElectrodesState},
            {"Respiration",     ChannelTypeRespiration},
            {"MEMS",            ChannelTypeMEMS},
            {"Orientation",     ChannelTypeOrientation},
            {"ConnectionStats", ChannelTypeConnectionStats},
            {"Resistance",      ChannelTypeResistance},
            {"Pedometer",       ChannelTypePedometer},
            {"Custom",          ChannelTypeCustom}
    };
}();


template<>
const std::unordered_map<DeviceState, std::string>
        enum_name_map<DeviceState>::mEnumToNameMap = []() {
    return std::unordered_map<DeviceState, std::string>{
            {DeviceStateConnected,    "Connected"},
            {DeviceStateDisconnected, "Disconnected"}
    };
}();

template<>
const std::unordered_map<std::string, DeviceState>
        enum_name_map<DeviceState>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, DeviceState>{
            {"Connected",    DeviceStateConnected},
            {"Disconnected", DeviceStateDisconnected}
    };
}();

template<>
const std::unordered_map<Command, std::string>
        enum_name_map<Command>::mEnumToNameMap = []() {
    return std::unordered_map<Command, std::string>{
            {CommandStartSignal,           "StartSignal"},
            {CommandStopSignal,            "StopSignal"},
            {CommandStartResist,           "StartResist"},
            {CommandStopResist,            "StopResist"},
            {CommandStartMEMS,             "StartMEMS"},
            {CommandStopMEMS,              "StopMEMS"},
            {CommandStartRespiration,      "StartRespiration"},
            {CommandStopRespiration,       "StopRespiration"},
            {CommandStartStimulation,      "StartStimulation"},
            {CommandStopStimulation,       "StopStimulation"},
            {CommandEnableMotionAssistant, "EnableMotionAssistant"},
            {CommandDisableMotionAssistant,"DisableMotionAssistant"},
            {CommandFindMe,                "FindMe"}
    };
}();

template<>
const std::unordered_map<std::string, Command>
        enum_name_map<Command>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, Command>{
            {"StartSignal",           CommandStartSignal},
            {"StopSignal",            CommandStopSignal},
            {"StartResist",           CommandStartResist},
            {"StopResist",            CommandStopResist},
            {"StartMEMS",             CommandStartMEMS},
            {"StopMEMS",              CommandStopMEMS},
            {"StartRespiration",      CommandStartRespiration},
            {"StopRespiration",       CommandStopRespiration},
            {"StartStimulation",      CommandStartStimulation},
            {"StopStimulation",       CommandStopStimulation},
            {"EnableMotionAssistant", CommandEnableMotionAssistant},
            {"DisableMotionAssistant",CommandDisableMotionAssistant},
            {"FindMe",                CommandFindMe}
    };
}();

template<>
const std::unordered_map<Parameter, std::string>
        enum_name_map<Parameter>::mEnumToNameMap = []() {
    return std::unordered_map<Parameter, std::string>{
            {ParameterState,                    "State"},
            {ParameterName,                     "Name"},
            {ParameterAddress,                  "Address"},
            {ParameterSerialNumber,             "SerialNumber"},
            {ParameterFirmwareMode,             "FirmwareMode"},
            {ParameterSamplingFrequency,        "SamplingFrequency"},
            {ParameterGain,                     "Gain"},
            {ParameterOffset,                   "Offset"},
            {ParameterHardwareFilterState,      "HardwareFilterState"},
            {ParameterExternalSwitchState,      "ExternalSwitchState"},
            {ParameterADCInputState,            "ADCInputState"},
            {ParameterGyroscopeSens,            "GyroscopeSens"},
            {ParameterAccelerometerSens,        "AccelerometerSens"},
            {ParameterStimulatorAndMAState,     "StimulatorAndMAState"},
            {ParameterStimulatorParamPack,      "StimulatorParamPack"},
            {ParameterMotionAssistantParamPack, "MotionAssistantParamPack"},
            {ParameterFirmwareVersion, "FirmwareVersion"}
    };
}();

template<>
const std::unordered_map<std::string, Parameter>
        enum_name_map<Parameter>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, Parameter>{
            {"State",                    ParameterState},
            {"Name",                     ParameterName},
            {"Address",                  ParameterAddress},
            {"SerialNumber",             ParameterSerialNumber},
            {"FirmwareMode",             ParameterFirmwareMode},
            {"SamplingFrequency",        ParameterSamplingFrequency},
            {"Gain",                     ParameterGain},
            {"Offset",                   ParameterOffset},
            {"HardwareFilterState",      ParameterHardwareFilterState},
            {"ExternalSwitchState",      ParameterExternalSwitchState},
            {"ADCInputState",            ParameterADCInputState},
            {"GyroscopeSens",            ParameterGyroscopeSens},
            {"AccelerometerSens",        ParameterAccelerometerSens},
            {"StimulatorAndMAState",     ParameterStimulatorAndMAState},
            {"StimulatorParamPack",      ParameterStimulatorParamPack},
            {"MotionAssistantParamPack", ParameterMotionAssistantParamPack},
            {"FirmwareVersion", ParameterFirmwareVersion}
    };
}();

template<>
const std::unordered_map<ParamAccess, std::string>
        enum_name_map<ParamAccess>::mEnumToNameMap = []() {
    return std::unordered_map<ParamAccess, std::string>{
            {Read,       "Read"},
            {ReadWrite,  "ReadWrite"},
            {ReadNotify, "ReadNotify"}
    };
}();

template<>
const std::unordered_map<std::string, ParamAccess>
        enum_name_map<ParamAccess>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, ParamAccess>{
            {"Read",       Read},
            {"ReadWrite",  ReadWrite},
            {"ReadNotify", ReadNotify}
    };
}();

template<>
const std::unordered_map<Filter, std::string>
        enum_name_map<Filter>::mEnumToNameMap = []() {
    return std::unordered_map<Filter, std::string>{
            {LowPass_1Hz_SF125,             "LowPass_1Hz_SF125"},
            {LowPass_1Hz_SF125_Reverse,     "LowPass_1Hz_SF125_Reverse"},
            {LowPass_5Hz_SF125_Reverse,     "LowPass_5Hz_SF125_Reverse"},
            {LowPass_15Hz_SF125,            "LowPass_15Hz_SF125"},
            {LowPass_15Hz_SF125_Reverse,    "LowPass_15Hz_SF125_Reverse"},
            {LowPass_27Hz_SF125,            "LowPass_27Hz_SF125"},
            {LowPass_27Hz_SF125_Reverse,    "LowPass_27Hz_SF125_Reverse"},
            {LowPass_30Hz_SF250,            "LowPass_30Hz_SF250"},
            {LowPass_30Hz_SF250_Reverse,    "LowPass_30Hz_SF250_Reverse"},
            {HighPass_2Hz_SF250,            "HighPass_2Hz_SF250"},
            {HighPass_2Hz_SF250_Reverse,    "HighPass_2Hz_SF250_Reverse"},
            {HighPass_3Hz_SF125,            "HighPass_3Hz_SF125"},
            {HighPass_3Hz_SF125_Reverse,    "HighPass_3Hz_SF125_Reverse"},
            {HighPass_5Hz_SF125,            "HighPass_5Hz_SF125"},
            {HighPass_5Hz_SF125_Reverse,    "HighPass_5Hz_SF125_Reverse"},
            {HighPass_11Hz_SF125,           "HighPass_11Hz_SF125"},
            {HighPass_11Hz_SF125_Reverse,   "HighPass_11Hz_SF125_Reverse"},
            {BandStop_45_55Hz_SF250,        "BandStop_45_55Hz_SF250"},
            {LowPass_5Hz_SF125,             "LowPass_5Hz_SF125"}
    };
}();

template<>
const std::unordered_map<std::string, Filter>
        enum_name_map<Filter>::mNameToEnumMap = []() {
    return std::unordered_map<std::string, Filter>{
            {"LowPass_1Hz_SF125",           LowPass_1Hz_SF125},
            {"LowPass_1Hz_SF125_Reverse",   LowPass_1Hz_SF125_Reverse},
            {"LowPass_5Hz_SF125_Reverse",   LowPass_5Hz_SF125_Reverse},
            {"LowPass_15Hz_SF125",          LowPass_15Hz_SF125},
            {"LowPass_15Hz_SF125_Reverse",  LowPass_15Hz_SF125_Reverse},
            {"LowPass_27Hz_SF125",          LowPass_27Hz_SF125},
            {"LowPass_27Hz_SF125_Reverse",  LowPass_27Hz_SF125_Reverse},
            {"LowPass_30Hz_SF250",          LowPass_30Hz_SF250},
            {"LowPass_30Hz_SF250_Reverse",  LowPass_30Hz_SF250_Reverse},
            {"HighPass_2Hz_SF250",          HighPass_2Hz_SF250},
            {"HighPass_2Hz_SF250_Reverse",  HighPass_2Hz_SF250_Reverse},
            {"HighPass_3Hz_SF125",          HighPass_3Hz_SF125},
            {"HighPass_3Hz_SF125_Reverse",  HighPass_3Hz_SF125_Reverse},
            {"HighPass_5Hz_SF125",          HighPass_5Hz_SF125},
            {"HighPass_5Hz_SF125_Reverse",  HighPass_5Hz_SF125_Reverse},
            {"HighPass_11Hz_SF125",         HighPass_11Hz_SF125},
            {"HighPass_11Hz_SF125_Reverse", HighPass_11Hz_SF125_Reverse},
            {"BandStop_45_55Hz_SF250",      BandStop_45_55Hz_SF250},
            {"LowPass_5Hz_SF125",           LowPass_5Hz_SF125}
    };
}();