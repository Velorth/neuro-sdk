#ifndef MEMS_DEVICE_PARAMS_H
#define MEMS_DEVICE_PARAMS_H

#include <stdexcept>

namespace Neuro {

enum class AccelerometerSensitivity {
    Sens2g,
    Sens4g,
    Sens8g,
    Sens16g
};

enum class GyroscopeSensitivity {
    Sens250Grad,
    Sens500Grad,
    Sens1000Grad,
    Sens2000Grad
};

template<typename T>
double doubleValue(T value);

template<>
inline double doubleValue<AccelerometerSensitivity>(AccelerometerSensitivity value) {
    switch (value) {
        case AccelerometerSensitivity::Sens2g:
            return 16.384;
        case AccelerometerSensitivity::Sens4g:
            return 8.192;
        case AccelerometerSensitivity::Sens8g:
            return 4.096;
        case AccelerometerSensitivity::Sens16g:
            return 2.048;
        default:
            throw std::runtime_error("Unsupported accelerometer sens value");
    }
}

template<>
inline double doubleValue<GyroscopeSensitivity>(GyroscopeSensitivity value) {
    switch (value) {
        case GyroscopeSensitivity::Sens250Grad:
            return 131;
        case GyroscopeSensitivity::Sens500Grad:
            return 65.5;
        case GyroscopeSensitivity::Sens1000Grad:
            return 32.8;
        case GyroscopeSensitivity::Sens2000Grad:
            return 16.4;
        default:
            throw std::runtime_error("Unsupported gyroscope sens value");
    }
}

}
#endif // MEMS_DEVICE_PARAMS_H
