#ifndef MEMS_DATA_H
#define MEMS_DATA_H

namespace Neuro {

struct MEMS{
    struct Accelerometer{
        double X;
        double Y;
        double Z;
    } accelerometer;

    struct Gyroscope{
        double X;
        double Y;
        double Z;
    } gyroscope;
};

}

#endif // MEMS_DATA_H
