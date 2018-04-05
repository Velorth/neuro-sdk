#ifndef CALLIBRI_BUFFER_H
#define CALLIBRI_BUFFER_H

#include "common_types.h"

namespace Neuro {

template <unsigned long MaxNumber>
class CallibriBuffer {
public:
    packet_number_t packetsLost() const noexcept {
        return mPacketsLost;
    }

    packet_number_t packetsReceived()const noexcept {
        return mPacketsReceived;
    }

protected:
    CallibriBuffer() = default;
    ~CallibriBuffer() = default;

    packet_number_t mPacketsLost;
    packet_number_t mPacketsReceived;

    void onNewPacket(packet_number_t packet_number){

    }
};

}

#endif // CALLIBRI_BUFFER_H
