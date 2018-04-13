#ifndef DEVICE_PARAMETERS_H
#define DEVICE_PARAMETERS_H

#include "ma_device_params.h"
#include "signal_device_params.h"

namespace Neuro {

enum class FirmwareMode {
    Application,
    Bootloader
};

enum class DeviceState {
    Disconnected,
    Connected,
    Working
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
    EnableMotionAssistant
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
    StimulatorState,
    MotionAssistantState,
    StimulatorParamPack,
    MotionAssistantParamPack
};

enum class ParamAccess {
    Read,
    ReadWrite
};

template <Parameter>
struct ParamValue;

}

#endif // DEVICE_PARAMETERS_H
