#include <string>
#include "device/device_parameters.h"

namespace Neuro {

std::string to_string(const DeviceState & state){
    switch(state){
    case DeviceState::Connected:
        return "Connected";
    case DeviceState::Disconnected:
        return "Disconnected";
    }
    return std::string();
}

std::string to_string(const Command &command){
    switch(command){
    case Command::EnableMotionAssistant:
        return "EnableMotionAssistant";
    case Command::FindMe:
        return "FindMe";
    case Command::StartMEMS:
        return "StartMEMS";
    case Command::StartResist:
        return "StartResist";
    case Command::StartRespiration:
        return "StartRespiration";
    case Command::StartSignal:
        return "StartSignal";
    case Command::StartStimulation:
        return "StartStimulation";
    case Command::StopMEMS:
        return "StopMEMS";
    case Command::StopResist:
        return "StopResist";
    case Command::StopRespiration:
        return "StopRespiration";
    case Command::StopSignal:
        return "StopSignal";
    }
    return std::string();
}

std::string to_string(const Parameter &param){
    switch(param){
    case Parameter::Name:
        return "Name";
    case Parameter::State:
        return "State";
    case Parameter::Address:
        return "Address";
    case Parameter::SerialNumber:
        return "SerialNumber";
    case Parameter::HardwareFilterState:
        return "HardwareFilterState";
    case Parameter::FirmwareMode:
        return "FirmwareMode";
    case Parameter::SamplingFrequency:
        return "SamplingFrequency";
    case Parameter::Gain:
        return "Gain";
    case Parameter::Offset:
        return "Offset";
    case Parameter::ExternalSwitchState:
        return "ExternalSwitchState";
    case Parameter::ADCInputState:
        return "ADCInputState";
    case Parameter::AccelerometerSens:
        return "AccelerometerSens";
    case Parameter::GyroscopeSens:
        return "GyroscopeSens";
    case Parameter::StimulatorAndMAState:
        return "StimulatorAndMAState";
    case Parameter::StimulatorParamPack:
        return "StimulatorParamPack";
    case Parameter::MotionAssistantParamPack:
        return "MotionAssistantParamPack";
    case Parameter::FirmwareVersion:
            return "FirmwareVersion";
    }
    return std::string();
}

std::string to_string(const ParamAccess &param_access){
    switch(param_access){
    case ParamAccess::Read:
        return "Read";
    case ParamAccess::ReadNotify:
        return "ReadNotify";
    case ParamAccess::ReadWrite:
        return "ReadWrite";
    }
    return std::string();
}

}
