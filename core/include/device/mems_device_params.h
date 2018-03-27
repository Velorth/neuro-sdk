#ifndef MEMS_DEVICE_PARAMS_H
#define MEMS_DEVICE_PARAMS_H

enum class AccelerometerSensitivity {
    Sens2g,
    Sens4g,
    Sens8g,
    Sens16g
};

enum class GyroscopeSensitivity : unsigned char {
    Sens250Grad,
    Sens500Grad,
    Sens1000Grad,
    Sens2000Grad
};

#endif // MEMS_DEVICE_PARAMS_H
