#ifndef CALLIBRI_SIGNAL_HANDLER_H
#define CALLIBRI_SIGNAL_HANDLER_H

#include "callibri_protocol.h"
#include "device/packet_handler.h"

namespace Neuro {

class CallibriSignalBuffer;

class CallibriSignalHandler : public PacketHandler<CallibriPacketType> {
public:
    CallibriSignalHandler(std::shared_ptr<CallibriSignalBuffer> buffer);

private:
    static constexpr std::size_t SignalPacketNumberPos = 0;
    static constexpr std::size_t SignalDataShift = 2;

    std::shared_ptr<CallibriSignalBuffer> mBuffer;

    bool processElement(const element_type &) override;
};
#endif // CALLIBRI_SIGNAL_HANDLER_H
