#include "device/callibri/callibri_signal_handler.h"
#include "device/callibri/callibri_signal_buffer.h"

namespace Neuro {

CallibriSignalHandler::CallibriSignalHandler(std::shared_ptr<CallibriSignalBuffer> buffer) :
    PacketHandler(CallibriPacketType::Signal),
    mBuffer(buffer){

}

bool CallibriSignalHandler::processElement(const element_type &packet){
    ByteInterpreter<packet_number_t> packetNumber;
    packetNumber.bytes[0] = packet[SignalPacketNumberPos];
    packetNumber.bytes[1] = packet[SignalPacketNumberPos + 1];
    ByteBuffer signalData(data.begin() + SignalDataShift, data.end());
    mBuffer->onDataReceived(packetNumber.value, signalData);
}

}
