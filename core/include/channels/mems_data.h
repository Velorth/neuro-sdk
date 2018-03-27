#ifndef MEMS_DATA_H
#define MEMS_DATA_H

namespace Neuro {

struct MEMS{
    struct Accelerometer{
        short X;
        short Y;
        short Z;
    } accelerometer;

    struct Gyroscope{
        short X;
        short Y;
        short Z;
    } gyroscope;
};

}

#endif // MEMS_DATA_H
