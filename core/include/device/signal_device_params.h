#ifndef SIGNAL_DEVICE_PARAMS_H
#define SIGNAL_DEVICE_PARAMS_H

namespace Neuro {

enum class SamplingFrequency {
    Hz125 = 0,
    Hz250 = 1,
    Hz500 = 2,
    Hz1000 = 3,
    Hz2000 = 4,
    Hz4000 = 5,
    Hz8000 = 6
};

inline unsigned getSamplingFrequencyValue(SamplingFrequency code){
    switch(code){
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
        return 0;
    }
}

enum class Gain {
    Gain1 = 0,
    Gain2 = 1,
    Gain3 = 2,
    Gain4 = 3,
    Gain6 = 4,
    Gain8 = 5,
    Gain12 = 6
};

inline unsigned getGainValue(Gain code){
    switch(code){
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
        return 0;
    }
}

enum class ExternalSwitchInput {
    MioElectrodesRespUSB = 0,
    MioElectrodes = 1,
    MioUSB = 2,
    RespUSB = 3
};

enum class ADCInput {
    Electrodes = 0,
    Short = 1,
    Test = 2,
    Resistance = 3
};

}

#endif // SIGNAL_DEVICE_PARAMS_H
