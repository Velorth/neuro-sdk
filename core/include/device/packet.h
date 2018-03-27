#ifndef PACKET_H
#define PACKET_H

#include <type_traits>
#include "common_types.h"

namespace Neuro {

template <typename PacketIdentificator>
class Packet {
public:
    template <typename T>
    Packet(PacketIdentificator id, T &&bytes) :
        mPacketId(id),
        mPacketBytes(std::forward<T>(bytes)){
        static_assert(std::is_enum<PacketIdentificator>::value,
                      "Packet identificator must be of enum type");
    }

    ByteBuffer packetBytes() const {
        return mPacketBytes;
    }

    PacketIdentificator id() const noexcept {
        return mPacketId;
    }

private:
    PacketIdentificator mPacketId;
    ByteBuffer mPacketBytes;
};

}

#endif // PACKET_H
