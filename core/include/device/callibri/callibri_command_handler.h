#ifndef CALLIBRI_COMMAND_HANDLER_H
#define CALLIBRI_COMMAND_HANDLER_H

#include "callibri_command.h"
#include "callibri_protocol.h"
#include "device/packet_handler.h"
#include "device/request_handler.h"

namespace Neuro {

class CallibriCommandHandler : public PacketHandler<CallibriPacketType> {
public:
    CallibriCommandHandler(std::shared_ptr<RequestHandler<CallibriCommandData>>);

private:
    static constexpr const char *class_name = "CallibriCommandHandler";
    static constexpr std::size_t CallibriCmdCodePos = 3;
    static constexpr std::size_t CallibriAddressPos = 4;
    static constexpr std::size_t CallibriChecksumPos = 7;
    static constexpr std::size_t CallibriDataStartPos = 8;
    static constexpr std::size_t CallibriAddressLength = 3;
    static constexpr long CallibriHostAddress = 0xA5B6C7L;

    std::shared_ptr<RequestHandler<CallibriCommandData>> mRequestHandler;

    bool processElement(const element_type &) override;
};

}

#endif // CALLIBRI_COMMAND_HANDLER_H
