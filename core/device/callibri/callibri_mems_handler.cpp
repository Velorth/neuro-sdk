#include "device/callibri/callibri_mems_handler.h"
#include "device/callibri/callibri_mems_buffer.h"

namespace Neuro {

CallibriMemsHandler::CallibriMemsHandler(std::shared_ptr<CallibriMemsBuffer> buffer) :
    PacketHandler(CallibriPacketType::MEMS),
    mBuffer(buffer){

}

bool CallibriMemsHandler::processElement(const element_type &packet){
    ByteInterpreter<packet_number_t> packetNumber;
    packetNumber.bytes[0] = packet[MemsPacketNumberPos];
    packetNumber.bytes[1] = packet[MemsPacketNumberPos + 1];
    ByteBuffer memsData(data.begin() + MemsDataShift, data.end());
    mBuffer->onDataReceived(packetNumber.value, memsData);
}

}
