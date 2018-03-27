#ifndef SIGNAL_DEVICE_PARAMS_H
#define SIGNAL_DEVICE_PARAMS_H

namespace Neuro {

enum class SamplingFrequency {
    Hz125,
    Hz250,
    Hz500,
    Hz1000,
    Hz2000,
    Hz4000,
    Hz8000
};

enum class Gain {
    Gain1,
    Gain2,
    Gain3,
    Gain4,
    Gain6,
    Gain8,
    Gain12
};

enum class ExternalSwitchInput {
    MioElectrodesRespUSB,
    MioElectrodes,
    MioUSB,
    RespUSB
};

enum class ADCInput {
    Electrodes,
    Short,
    Test,
    Resistance
};

template <typename T>
int intValue(T value);

template <>
inline int intValue<Gain>(Gain value){
    switch (value)
    {
        case Gain::Gain1:
            return 1;
        case Gain::Gain2:
            return 2;
        case Gain::Gain3:
            return 3;
        case Gain::Gain4:
            return 4;
        case Gain::Gain6:
            return 6;
        case Gain::Gain8:
            return 8;
        case Gain::Gain12:
            return 12;
        default:
            throw std::runtime_error("Unsupported gain value");
    }
}

template <>
inline int intValue<SamplingFrequency>(SamplingFrequency value){
    switch (value)
    {
        case SamplingFrequency::Hz125:
            return 125;
        case SamplingFrequency::Hz250:
            return 250;
        case SamplingFrequency::Hz500:
            return 500;
        case SamplingFrequency::Hz1000:
            return 1000;
        case SamplingFrequency::Hz2000:
            return 2000;
        case SamplingFrequency::Hz4000:
            return 4000;
        case SamplingFrequency::Hz8000:
            return 8000;
        default:
            throw std::runtime_error("Unsupported sampling frequency value");
    }
}

}

#endif // SIGNAL_DEVICE_PARAMS_H
