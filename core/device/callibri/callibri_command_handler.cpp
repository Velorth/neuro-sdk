#include <algorithm>
#include "device/callibri/callibri_command_handler.h"

namespace Neuro {

CallibriCommandHandler::CallibriCommandHandler(std::shared_ptr<RequestHandler<CallibriCommandData>> requestHandler) :
    PacketHandler(CallibriPacketType::Command),
    mRequestHandler(requestHandler){

}

bool CallibriCommandHandler::processElement(const element_type &packet){
    auto packetBytes = packet.packetBytes();
    char checksum = 0;
    for (auto headerByte =packetBytes.data();
         headerByte != packetBytes.data() + CallibriDataStartPos; checksum += *headerByte++);

    LOG_DEBUG_V("Checksum: %d", checksum);

    if (checksum) {
        LOG_WARN("Checksum isn't equals to zero. Discarding packet.");
        return false;
    }

    ByteInterpreter<long> hostAddress;
    hostAddress.value = 0;
    std::copy(packetBytes.data() + CallibriAddressPos,
         packetBytes.data() + CallibriAddressPos + CallibriAddressLength, hostAddress.bytes);

    if (hostAddress.value!=CallibriHostAddress){
        LOG_ERROR_V("Host address is not valid: %ld. Must be %ld", hostAddress.value, CallibriHostAddress);
        return false;
    }

    LOG_DEBUG("Command response received");
    CallibriCommand command;
    if (!parseCommand(packetBytes[CallibriCmdCodePos], &command))
    {
        LOG_DEBUG_V("Command parsing failed for command code %d", packetBytes[CallibriCmdCodePos]);
        return false;
    }

    LOG_DEBUG("Processing response");
    mRequestHandler->onCommandResponse(command,
                                       packetBytes.data() + CallibriDataStartPos,
                                       packetBytes.size() - CallibriDataStartPos);

    return true;
}

}
