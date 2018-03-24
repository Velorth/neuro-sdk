#include <device/callibri/callibri_protocol.h>
#include "channels/channel_info.h"
#include "device/device_parameters.h"
#include "device/callibri/callibri_impl.h"
#include "device/callibri/callibri_parameter_reader.h"
#include "device/callibri/callibri_parameter_writer.h"
#include "device/callibri/callibri_common_parameters.h"
#include "device/callibri/callibri_protocol.h"
#include "device/request_handler.h"
#include "signal/circular_buffer.h"

namespace Neuro {

CallibriImpl::CallibriImpl(std::shared_ptr<BleDevice> ble_device,
                           std::shared_ptr<CallibriRequestHandler> request_handler,
                           std::shared_ptr<CallibriCommonParameters> common_params) :
    DeviceImpl(ble_device,
               std::make_unique<CallibriParameterReader>(ble_device,
                                                         [=](Parameter param){
                                                             onParameterChanged(param);
                                                         },
                                                         common_params,
                                                         request_handler),
               std::make_unique<CallibriParameterWriter>(common_params)),
    mRequestHandler(request_handler),
    mCommonParams(common_params),
    mSignalBuffer(mCommonParams){
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
        return false;
    }
    case Command::StopSignal:{
        return false;
    }
    case Command::StartMEMS:{
        return false;
    }
    case Command::StopMEMS:{
        return false;
    }
    case Command::StartRespiration:{
        return false;
    }
    case Command::StopRespiration:{
        return false;
    }
    case Command::EnableMotionAssistant:{
        return false;
    }
    case Command::StartStimulation:{
        return false;
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
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::GET_ELECTRODE_STATE);
    log->debug("[%s: %s] Sending electrode state request", "CallibriImpl", __FUNCTION__);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto error = cmdData->getError();
    if (error != ColibriCommandError::NO_ERROR) {
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

const BaseBuffer<signal_sample_t> &CallibriImpl::signalBuffer() const {
    return mSignalBuffer.buffer();
}

const BaseBuffer<signal_sample_t> &CallibriImpl::respirationBuffer() const {
    return mRespirationBuffer.buffer();
}

void CallibriImpl::onDataReceived(const ByteBuffer &data){
    auto log = LoggerFactory::getCurrentPlatformLogger();

    log->trace("[%s: %s] Data received. Length: %zd", "CallibriImpl", __FUNCTION__, data.size());
    if (data.size() < COLIBRI_PACKET_SIZE) return;

    ColibriUShort marker;
    marker.bytes[0] = data[0];
    marker.bytes[1] = data[1];

    log->trace("[%s: %s] Marker: %d", "CallibriImpl", __FUNCTION__, marker.value);

    //Signal data received
    if (marker.value <= COLIBRI_DATA_MAX_PACKET_NMB){
        mSignalBuffer.onSignalReceived(marker.value,
                                         ByteBuffer(data.begin() + COLIBRI_SIGNAL_DATA_START_POS,
                                                    data.end()));
    }
    //Colibri command response received
    else if (marker.value == COLIBRI_COMMAND_MARKER){
        log->debug("[%s: %s] Command marker", "CallibriImpl", __FUNCTION__);
        char checksum = 0;
        for (auto headerByte = data.data();
             headerByte != data.data() + COLIBRI_CMD_HDR_DATA_START_POS; checksum += *headerByte++);

        log->debug("[%s: %s] Checksum: %d", "CallibriImpl", __FUNCTION__,checksum);

        //if checksum is not 0 discard packet
        if (checksum) {
            log->warn("[%s: %s] Checksum isn't equals to zero. Discarding packet.", "CallibriImpl", __FUNCTION__);
            return;
        }

        ColibriLong hostAddress;
        hostAddress.value = 0;
        std::copy(data.data() + COLIBRI_HEADER_ADDR_POSITION,
             data.data() + COLIBRI_HEADER_ADDR_POSITION + COLIBRI_ADDRESS_LENGTH, hostAddress.bytes);

        //if host address is not valid discard packet
        if (hostAddress.value!=COLIBRI_HOST_ADDRESS){
            log->error("[%s: %s] Host address is not valid: %ld. Must be %ld", "CallibriImpl", __FUNCTION__, hostAddress.value, COLIBRI_HOST_ADDRESS);
            return;
        }

        onCommandPacketReceived(data.data(), data.size());
    }
    //Spirometry data received
    else if (marker.value == COLIBRI_SPIRO_DATA_MARKER){
        mRespirationBuffer.onSignalReceived(marker.value,
                                            ByteBuffer(data.begin() + COLIBRI_SIGNAL_DATA_START_POS,
                                                       data.end()));
    }
    //MEMS data received
    else if (marker.value == COLIBRI_MEMS_DATA_MARKER){
        //onMemsDataReceived(data,length);
    }
}

void CallibriImpl::onCommandPacketReceived(const unsigned char *const command_packet, size_t length){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Command response received", "CallibriImpl", __FUNCTION__);
    ColibriCommand command;
    if (!parseCommand(command_packet[COLIBRI_HEADER_CMD_POSITION], &command))
    {
        log->debug("[%s: %s] Command parsing failed for command code %d", "CallibriImpl", __FUNCTION__,command_packet[COLIBRI_HEADER_CMD_POSITION]);
        return;
    }

    log->debug("[%s: %s] Processing response", "CallibriImpl", __FUNCTION__);
    mRequestHandler->onCommandResponse(command,
                                       command_packet + COLIBRI_CMD_HDR_DATA_START_POS,
                                       length - COLIBRI_CMD_HDR_DATA_START_POS);
}

void CallibriImpl::sendCommandPacket(std::shared_ptr<CallibriCommandData> commandData){
    std::vector<Byte> packet(COLIBRI_PACKET_SIZE);

    ColibriUShort commandMarker;
    commandMarker.value = COLIBRI_COMMAND_MARKER;

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Sending command packet for command %d", "CallibriImpl", __FUNCTION__, commandData->getCommand());

    packet[0] = commandMarker.bytes[0];
    packet[1] = commandMarker.bytes[1];
    packet[COLIBRI_HEADER_CMD_POSITION] = static_cast<unsigned char>(commandData->getCommand());

    ColibriLong address;
    address.value = mCommonParams->callibriAddress();
    packet[COLIBRI_HEADER_ADDR_POSITION] = address.bytes[0];
    packet[COLIBRI_HEADER_ADDR_POSITION + 1] = address.bytes[1];
    packet[COLIBRI_HEADER_ADDR_POSITION + 2] = address.bytes[2];

    unsigned char checksum = 0;
    for (auto packetByte = &packet[0];
         packetByte != &packet[0] + COLIBRI_HEADER_CS_POSITION; checksum -= *packetByte++);
    packet[COLIBRI_HEADER_CS_POSITION] = checksum;

    if (auto dataLength = commandData->getRequestLength())
    {
        if (COLIBRI_CMD_HDR_DATA_START_POS + dataLength > COLIBRI_PACKET_SIZE)
            return;

        auto requestData = commandData->getRequestData();
        if (requestData.size() > COLIBRI_PACKET_SIZE + COLIBRI_CMD_HDR_DATA_START_POS)
        {
            log->warn("[%s: %s] Packet data array too big!", "CallibriImpl", __FUNCTION__);
            return;
        }

        std::copy(requestData.begin(), requestData.end(), packet.begin()+COLIBRI_CMD_HDR_DATA_START_POS);
    }

    mBleDevice->sendCommand(packet);
    log->debug("[%s: %s] Command been sent", "CallibriImpl", __FUNCTION__);
}

int CallibriImpl::requestBattryVoltage(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<CallibriCommandData>(ColibriCommand::GET_BATTERY_V);
    log->debug("[%s: %s] Sending battery level request", "CallibriImpl", __FUNCTION__);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto error = cmdData->getError();
    if (error != ColibriCommandError::NO_ERROR) {
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

void CallibriImpl::onParameterChanged(Parameter param) {
    if (parameterChangedCallback){
        parameterChangedCallback(param);
    }
}

}
