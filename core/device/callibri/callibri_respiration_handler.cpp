#include "device/callibri/callibri_respiration_handler.h"
#include "device/callibri/callibri_respiration_buffer.h"

namespace Neuro {

CallibriRespirationHandler::CallibriRespirationHandler(std::shared_ptr<CallibriRespirationBuffer> buffer) :
    PacketHandler(CallibriPacketType::Respiration),
    mBuffer(buffer){

}

bool CallibriRespirationHandler::processElement(const element_type &packet){
    ByteInterpreter<packet_number_t> packetNumber;
    packetNumber.bytes[0] = packet[RespPacketNumberPos];
    packetNumber.bytes[1] = packet[RespPacketNumberPos + 1];
    ByteBuffer memsData(data.begin() + RespDataShift, data.end());
    mBuffer->onDataReceived(packetNumber.value, memsData);
}

}
