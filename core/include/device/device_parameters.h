#ifndef DEVICE_PARAMETERS_H
#define DEVICE_PARAMETERS_H

#include "ma_device_params.h"
#include "mems_device_params.h"
#include "signal_device_params.h"
#include "lib_export.h"
#include <utility>

namespace Neuro {

enum class FirmwareMode {
    Application,
    Bootloader
};

enum class DeviceState {
    Disconnected,
    Connected
};

enum class Command {
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
};

enum class Parameter {
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
};

enum class ParamAccess {
    Read,
    ReadWrite,
    ReadNotify
};

SDK_SHARED std::string to_string(const DeviceState &);
SDK_SHARED std::string to_string(const Command &);
SDK_SHARED std::string to_string(const Parameter &);
SDK_SHARED std::string to_string(const ParamAccess &);

template <Parameter>
struct ParamValue;

using ParamPair = std::pair<Parameter, ParamAccess>;

}


#endif // DEVICE_PARAMETERS_H
