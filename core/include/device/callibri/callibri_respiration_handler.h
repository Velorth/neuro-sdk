#ifndef CALLIBRI_RESPIRATION_HANDLER_H
#define CALLIBRI_RESPIRATION_HANDLER_H

#include "callibri_protocol.h"
#include "device/packet_handler.h"

namespace Neuro {

class CallibriRespirationBuffer;

class CallibriRespirationHandler : public PacketHandler<CallibriPacketType> {
public:
    CallibriRespirationHandler(std::shared_ptr<CallibriRespirationBuffer> buffer);

private:
    static constexpr std::size_t RespPacketNumberPos = 2;
    static constexpr std::size_t RespDataShift = 4;

    std::shared_ptr<CallibriRespirationBuffer> mBuffer;

    bool processElement(const element_type &) override;
};

#endif // CALLIBRI_RESPIRATION_HANDLER_H
