#include "channels/channel_info.h"
#include "device/device_parameters.h"
#include "device/callibri/callibri_impl.h"
#include "device/callibri/callibri_parameter_reader.h"
#include "device/callibri/callibri_parameter_writer.h"
#include "device/callibri/callibri_common_parameters.h"
#include "device/callibri/callibri_protocol.h"
#include "device/packet.h"
#include "device/request_handler.h"

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
    mCommonParams(common_params){
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

const BaseBuffer<signal_sample_t> &CallibriImpl::signalBuffer() const {
    /*auto bufferPtr = mSignalBuffer.lock();
    if (bufferPtr == nullptr){
        throw std::runtime_error("Device does not have signal buffer");
    }
    return *bufferPtr;*/
}

const BaseBuffer<resp_sample_t> &CallibriImpl::respirationBuffer() const {
    /*auto bufferPtr = mRespirationBuffer.lock();
    if (bufferPtr == nullptr){
        throw std::runtime_error("Device does not have respiration buffer");
    }
    return *bufferPtr;*/
}

const BaseBuffer<MEMS> &CallibriImpl::memsBuffer() const {
   /* auto bufferPtr = mMemsBuffer.lock();
    if (bufferPtr == nullptr){
        throw std::runtime_error("Device does not have MEMS buffer");
    }
    return *bufferPtr;*/
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
    auto packetType = fromMarker(marker.value);
    Packet<CallibriPacketType> packet(packetType, data);
    if (!mPacketHandler->process(packet)){
        LOG_ERROR("Failed to process packet");
    }
}

void CallibriImpl::sendCommandPacket(std::shared_ptr<CallibriCommandData> commandData){
    std::vector<Byte> packet(COLIBRI_PACKET_SIZE);

    ByteInterpreter<unsigned short> commandMarker;
    commandMarker.value = COLIBRI_COMMAND_MARKER;

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Sending command packet for command %d", "CallibriImpl", __FUNCTION__, commandData->getCommand());

    packet[0] = commandMarker.bytes[0];
    packet[1] = commandMarker.bytes[1];
    packet[COLIBRI_HEADER_CMD_POSITION] = static_cast<unsigned char>(commandData->getCommand());

    ByteInterpreter<long> serial;
    serial.value = mCommonParams->serialNumber();
    packet[COLIBRI_HEADER_ADDR_POSITION] = serial.bytes[0];
    packet[COLIBRI_HEADER_ADDR_POSITION + 1] = serial.bytes[1];
    packet[COLIBRI_HEADER_ADDR_POSITION + 2] = serial.bytes[2];

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

void CallibriImpl::onParameterChanged(Parameter param) {
    if (parameterChangedCallback){
        parameterChangedCallback(param);
    }
}

}
