#ifndef PACKET_SEQUENCE_H
#define PACKET_SEQUENCE_H

#include "common_types.h"

namespace Neuro {

template <packet_number_t Max>
class PacketSequence{
public:
    packet_number_t onNewPacket(packet_number_t packet_number){
        auto totalPackets = mPacketsReceived + mPacketsLost;
        auto totalModMax = totalPackets % Max;

        if (packet_number >= totalModMax){
            throw std::runtime_error("Packet sequence flow violation");
        }

        long span = packet_number - totalModMax;
        auto packetsLost = span - 1;
        mPacketsLost += packetsLost;
        ++mPacketsReceived;
        return packetsLost;
    }

    packet_number_t packetsLost() const noexcept {
        return mPacketsLost;
    }

    packet_number_t packetsReceived() const noexcept {
        return mPacketsReceived;
    }

private:
    packet_number_t mPacketsLost{0};
    packet_number_t mPacketsReceived{0};
};

}

#endif // PACKET_SEQUENCE_H
