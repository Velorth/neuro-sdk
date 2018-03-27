#ifndef CALLIBRI_MEMS_HANDLER_H
#define CALLIBRI_MEMS_HANDLER_H

#include "callibri_protocol.h"
#include "device/packet_handler.h"

namespace Neuro {

class CallibriMemsBuffer;

class CallibriMemsHandler : public PacketHandler<CallibriPacketType> {
public:
    CallibriMemsHandler(std::shared_ptr<CallibriMemsBuffer> buffer);

private:
    static constexpr std::size_t MemsPacketNumberPos = 2;
    static constexpr std::size_t MemsDataShift = 4;

    std::shared_ptr<CallibriMemsBuffer> mBuffer;

    bool processElement(const element_type &) override;
};

}
#endif // CALLIBRI_PACKET_HANDLER_H
