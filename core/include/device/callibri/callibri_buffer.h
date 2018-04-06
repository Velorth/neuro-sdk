#ifndef CALLIBRI_BUFFER_H
#define CALLIBRI_BUFFER_H

#include "device/packet_sequence.h"

namespace Neuro {

template <unsigned long MaxNumber>
class CallibriBuffer {
public:
    packet_number_t packetsLost() const noexcept {
        return mPacketSequence.packetsLost();
    }

    packet_number_t packetsReceived() const noexcept {
        return mPacketSequence.packetsReceived();
    }

protected:
    CallibriBuffer() = default;
    ~CallibriBuffer() = default;

    PacketSequence<MaxNumber> mPacketSequence;
};

}

#endif // CALLIBRI_BUFFER_H
