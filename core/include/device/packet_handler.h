#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include "packet.h"
#include "handler.h"

namespace Neuro {

template <typename PacketIdentificator>
class PacketHandler : public Handler<Packet<PacketIdentificator>> {
public:
    constexpr PacketHandler(PacketIdentificator id) :
        mPacketId(id){
    }

private:
    const PacketIdentificator mPacketId;

    bool isResponsible(const element_type &packet) const noexcept override {
        return packet.id() == mPacketId;
    }
};

}

#endif // PACKET_HANDLER_H
