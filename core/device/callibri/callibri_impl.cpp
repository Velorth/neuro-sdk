#include "channels/channel_info.h"
#include "device/device_parameters.h"
#include "device/callibri/callibri_buffer_collection.h"
#include "device/callibri/callibri_impl.h"
#include "device/callibri/callibri_parameter_reader.h"
#include "device/callibri/callibri_parameter_writer.h"
#include "device/callibri/callibri_common_parameters.h"
#include "device/callibri/callibri_protocol.h"
#include "device/packet.h"
#include "device/request_scheduler.h"

namespace Neuro {

CallibriImpl::CallibriImpl(std::shared_ptr<BleDevice> ble_device,
                           std::shared_ptr<CallibriRequestScheduler> request_handler,
                           std::shared_ptr<CallibriCommonParameters> common_params,
                           std::shared_ptr<CallibriBufferCollection> buffer_collection) :
    DeviceImpl(ble_device,
               std::make_unique<CallibriParameterReader>(ble_device,
                                                         [=](Parameter param){
                                                             onParameterChanged(param);
                                                         },
                                                         common_params,
                                                         request_handler,
                                                         buffer_collection),
               std::make_unique<CallibriParameterWriter>(common_params)),
    mRequestHandler(request_handler),
    mCommonParams(common_params),
    mBufferCollection(buffer_collection){
    mRequestHandler->setSendFunction([=](std::shared_ptr<CallibriCommandData> cmd_data){this->sendCommandPacket(cmd_data);});
}

std::vector<ChannelInfo> CallibriImpl::channels() const {
    return mCommonParams->availableChannels();
}

std::vector<Command> CallibriImpl::commands() const {
    return mCommonParams->availableCommands();
}

std::vector<ParamPair> CallibriImpl::parameters() const {
    return mCommonParams->availableParameters();
}

void CallibriImpl::setParamChangedCallback(param_changed_callback_t callback) {
   parameterChangedCallback = callback;
}

bool CallibriImpl::execute(Command command){
    auto availableCommands = mCommonParams->availableCommands();
    if (std::find(availableCommands.begin(), availableCommands.end(), command) == availableCommands.end()){
        throw std::runtime_error("Unsupported command");
    }
    switch (command){
    case Command::StartSignal:{
        return sendSimpleCommand<Command::StartSignal>();
    }
    case Command::StopSignal:{
        return sendSimpleCommand<Command::StopSignal>();
    }
    case Command::StartMEMS:{
        return sendSimpleCommand<Command::StartMEMS>();
    }
    case Command::StopMEMS:{
        return sendSimpleCommand<Command::StopMEMS>();
    }
    case Command::StartRespiration:{
        return sendSimpleCommand<Command::StartRespiration>();
    }
    case Command::StopRespiration:{
        return sendSimpleCommand<Command::StopRespiration>();
    }
    case Command::EnableMotionAssistant:{
        return sendSimpleCommand<Command::EnableMotionAssistant>();
    }
    case Command::StartStimulation:{
        return sendSimpleCommand<Command::StartStimulation>();
    }
    case Command::FindMe:{
        return sendSimpleCommand<Command::FindMe>();
    }
    default:
        throw std::runtime_error("Unsupported command");
    }
}

int CallibriImpl::batteryChargePercents(){
    auto voltage = requestBattryVoltage();
    return convertVoltageToPercents(voltage);
}

bool CallibriImpl::isElectrodesAttached(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::GET_ELECTRODE_STATE);
    log->debug("[%s: %s] Sending electrode state request", "CallibriImpl", __FUNCTION__);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto error = cmdData->getError();
    if (error != CallibriError::NO_ERROR) {
        log->warn("[%s: %s] Failed get electrode state. Error code: %d", "CallibriImpl",
                  __FUNCTION__, error);
        throw std::runtime_error("Unable get electrodes state due to connection error");
    }
    //electrode state consists of 1 bytes which represent byte
    auto responseLength = cmdData->getResponseLength();

    if (responseLength >= sizeof(unsigned char)) {
        unsigned char electrodeState;
        auto responseData = cmdData->getResponseData();
        std::copy(responseData.begin(), responseData.begin() + sizeof(unsigned char),
                  &electrodeState);

        log->debug("[%s: %s] Electrode state received: %d", "CallibriImpl", __FUNCTION__,
                   electrodeState);
        return !electrodeState;
    }
    else {
        log->warn("[%s: %s] Failed get electrode state. Response length is zero",
                  "CallibriImpl", __FUNCTION__);
        throw std::runtime_error("Unable get electrodes state due to connection error");
    }
}

std::size_t CallibriImpl::packetsLost(){
    return mBufferCollection->packetsLost();
}

std::size_t CallibriImpl::packetsReceived(){
    return mBufferCollection->packetsReceived();
}

const BaseBuffer<signal_sample_t> &CallibriImpl::signalBuffer() const {    
    return mBufferCollection->signalBuffer().buffer();
}

const BaseBuffer<resp_sample_t> &CallibriImpl::respirationBuffer() const {    
    return mBufferCollection->respirationBuffer().buffer();
}

const BaseBuffer<MEMS> &CallibriImpl::memsBuffer() const {    
    return mBufferCollection->memsBuffer().buffer();
}

const BaseBuffer<Quaternion> &CallibriImpl::angleBuffer() const {
    return mBufferCollection->angleBuffer().buffer();
}

void CallibriImpl::onDataReceived(const ByteBuffer &data){
    LOG_TRACE_V("Data received. Length: %zd", data.size());
    if (data.size() < CallibriPacketSize)
        return;

    static_assert(sizeof(callibri_marker_t) >= CallibriMarkerLength,
                  "Callibri marker does not fit into callibri_marker_t");

    ByteInterpreter<callibri_marker_t> marker;
    marker.bytes[0] = data[0];
    marker.bytes[1] = data[1];
    LOG_TRACE_V("Marker value: %d", marker.value);

    try {
        auto packetType = fromMarker(marker.value);
        switch (packetType) {
            case CallibriPacketType::Command: {
                onCommandResponse(data);
                break;
            }
            case CallibriPacketType::Signal: {
                onSignalReceived(data);
                break;
            }
            case CallibriPacketType::MEMS: {
                onMemsReceived(data);
                break;
            }
            case CallibriPacketType::Respiration: {
                onRespReceived(data);
                break;
            }
        }
    }
    catch (std::runtime_error &e){
        LOG_ERROR_V("Unable to parse packet: %s", e.what());
    }
}

int CallibriImpl::requestBattryVoltage(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<CallibriCommandData>(CallibriCommand::GET_BATTERY_V);
    log->debug("[%s: %s] Sending battery level request", "CallibriImpl", __FUNCTION__);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto error = cmdData->getError();
    if (error != CallibriError::NO_ERROR) {
        log->warn("[%s: %s] Failed get battery voltage. Error code: %d", "CallibriImpl", __FUNCTION__, error);
        throw std::runtime_error("Unable receive battery charge value due to communication error");
    }

    //battery voltage value consists of 2 bytes which represent unsigned short integer
    auto responseLength = cmdData->getResponseLength();

    if (responseLength >= sizeof(ByteInterpreter<unsigned short>::value)){
        ByteInterpreter<unsigned short> batteryVoltage;
        auto responseData = cmdData->getResponseData();
        std::copy(responseData.begin(), responseData.begin() + sizeof(ByteInterpreter<unsigned short>::value), batteryVoltage.bytes);

        log->debug("[%s: %s] Battery voltage received: %d", "CallibriImpl", __FUNCTION__, batteryVoltage.value);
        return batteryVoltage.value;
    }
    else{
        throw std::runtime_error("Unable receive battery charge value due to communication error");
    }
}

int CallibriImpl::convertVoltageToPercents(int voltage){
    auto batteryLevel = 0;
    if (voltage <= 3130) batteryLevel=(0);
    else if (voltage <= 3568) batteryLevel=(5);
    else if (voltage <= 3650) batteryLevel=(10);
    else if (voltage <= 3710) batteryLevel=(15);
    else if (voltage <= 3750) batteryLevel=(20);
    else if (voltage <= 3770) batteryLevel=(25);
    else if (voltage <= 3785) batteryLevel=(30);
    else if (voltage <= 3799) batteryLevel=(35);
    else if (voltage <= 3808) batteryLevel=(40);
    else if (voltage <= 3820) batteryLevel=(45);
    else if (voltage <= 3832) batteryLevel=(50);
    else if (voltage <= 3855) batteryLevel=(55);
    else if (voltage <= 3867) batteryLevel=(60);
    else if (voltage <= 3890) batteryLevel=(65);
    else if (voltage <= 3937) batteryLevel=(70);
    else if (voltage <= 3970) batteryLevel=(75);
    else if (voltage <= 3996) batteryLevel=(80);
    else if (voltage <= 4022) batteryLevel=(85);
    else if (voltage <= 4060) batteryLevel=(90);
    else if (voltage <= 4100) batteryLevel=(95);
    else batteryLevel=(100);
    return batteryLevel;
}

void CallibriImpl::onStatusDataReceived(const ByteBuffer &){
    throw std::logic_error("Status data is not suitable for Callibri device");
}

void CallibriImpl::onCommandResponse(const ByteBuffer &packetBytes){
    char checksum = 0;
    for (auto headerByte = packetBytes.data();
         headerByte != packetBytes.data() + CallibriDataStartPos; checksum += *headerByte++);

    LOG_DEBUG_V("Checksum: %d", checksum);

    if (checksum) {
        LOG_WARN("Checksum isn't equals to zero. Discarding packet.");
        return;
    }

    ByteInterpreter<long> hostAddress;
    hostAddress.value = 0;
    std::copy(packetBytes.data() + CallibriAddressPos,
              packetBytes.data() + CallibriAddressPos + CallibriAddressLength, hostAddress.bytes);

    if (hostAddress.value!=CallibriHostAddress){
        LOG_ERROR_V("Host address is not valid: %ld. Must be %ld", hostAddress.value, CallibriHostAddress);
        return;
    }

    LOG_DEBUG("Command response received");
    CallibriCommand command;
    if (!parseCommand(packetBytes[CallibriCmdCodePos], &command))
    {
        LOG_ERROR_V("Command parsing failed for command code %d", packetBytes[CallibriCmdCodePos]);
        return;
    }

    LOG_DEBUG("Processing response");
    mRequestHandler->onCommandResponse(command,
                                       packetBytes.data() + CallibriDataStartPos,
                                       packetBytes.size() - CallibriDataStartPos);
}

void CallibriImpl::onSignalReceived(const ByteBuffer &data){
    auto packetNumber = extractPacketNumber(data, SignalPacketNumberPos);
    ByteBuffer signalData(data.begin() + SignalDataShift, data.end());
    mBufferCollection->signalBuffer().onDataReceived(packetNumber, signalData);
}

void CallibriImpl::onMemsReceived(const ByteBuffer &data){
    auto packetNumber = extractPacketNumber(data, MemsPacketNumberPos);
    ByteBuffer memsData(data.begin() + MemsDataShift, data.end());
    mBufferCollection->memsBuffer().onDataReceived(packetNumber, memsData);
}

void CallibriImpl::onRespReceived(const ByteBuffer &data){
    auto packetNumber = extractPacketNumber(data, RespPacketNumberPos);
    ByteBuffer respData(data.begin() + RespDataShift, data.end());
   mBufferCollection->respirationBuffer().onDataReceived(packetNumber, respData);
}

packet_number_t CallibriImpl::extractPacketNumber(const ByteBuffer &packet, std::size_t number_pos){
    ByteInterpreter<packet_number_t> packetNumber;
    packetNumber.bytes[0] = packet[number_pos];
    packetNumber.bytes[1] = packet[number_pos + 1];
    return packetNumber.value;
}

void CallibriImpl::onParameterChanged(Parameter param) {
    if (parameterChangedCallback){
        parameterChangedCallback(param);
    }
}

void CallibriImpl::sendCommandPacket(std::shared_ptr<CallibriCommandData> commandData){
    std::vector<Byte> packet(CallibriPacketSize);

    ByteInterpreter<unsigned short> commandMarker;
    commandMarker.value = CallibriCommandMarker;

    LOG_DEBUG_V("Sending command packet for command %d", commandData->getCommand());

    packet[0] = commandMarker.bytes[0];
    packet[1] = commandMarker.bytes[1];
    packet[CallibriCmdCodePos] = static_cast<unsigned char>(commandData->getCommand());

    ByteInterpreter<long> serial;
    serial.value = mCommonParams->serialNumber();
    packet[CallibriAddressPos] = serial.bytes[0];
    packet[CallibriAddressPos + 1] = serial.bytes[1];
    packet[CallibriAddressPos + 2] = serial.bytes[2];

    unsigned char checksum = 0;
    for (auto packetByte = &packet[0];
         packetByte != &packet[0] + CallibriChecksumPos; checksum -= *packetByte++);
    packet[CallibriChecksumPos] = checksum;

    if (auto dataLength = commandData->getRequestLength())
    {
        if (CallibriDataStartPos + dataLength > CallibriPacketSize)
            return;

        auto requestData = commandData->getRequestData();
        if (requestData.size() > CallibriPacketSize - CallibriDataStartPos)
        {
            LOG_WARN("Packet data array too big!");
            return;
        }

        std::copy(requestData.begin(), requestData.end(), packet.begin()+CallibriDataStartPos);
    }

    mBleDevice->sendCommand(packet);
    LOG_DEBUG("Command been sent");
}

}
