#include "channels/channel_info.h"
#include "device/brainbit/brainbit_impl.h"
#include "device/device_parameters.h"
#include "device/brainbit/brainbit_parameter_reader.h"
#include "device/brainbit/brainbit_parameter_writer.h"
#include "device/brainbit/brainbit_protocol.h"
#include "signal/circular_buffer.h"

namespace Neuro {

BrainbitImpl::BrainbitImpl(std::shared_ptr<BleDevice> ble_device):
    DeviceImpl(ble_device,
               std::make_unique<BrainbitParameterReader>(ble_device,
                                                         [=](auto param){
                                                             onParameterChanged(param);
                                                         }),
               std::make_unique<BrainbitParameterWriter>()),
    mRequestHandler(std::make_unique<BrainbitRequestHandler>(
                        [=](std::shared_ptr<BrainbitCommandData> cmd){sendCommandPacket(cmd);})){

}

std::vector<ChannelInfo> BrainbitImpl::channels() const {
    auto t3Info = ChannelInfo(ChannelInfo::Type::Signal, "T3", 0);
    auto t4Info = ChannelInfo(ChannelInfo::Type::Signal, "T4", 1);
    auto o1Info = ChannelInfo(ChannelInfo::Type::Signal, "O1", 2);
    auto o2Info = ChannelInfo(ChannelInfo::Type::Signal, "O2", 3);
    return { ChannelInfo::Battery,
                ChannelInfo::ConnectionStats,
                t3Info,
                t4Info,
                o1Info,
                o2Info,
                ChannelInfo::Resistance };
}

std::vector<Command> BrainbitImpl::commands() const {
    return { Command::StartSignal,
                Command::StopSignal,
                Command::StartResist,
                Command::StopResist };
}

std::vector<ParamPair > BrainbitImpl::parameters() const {
    return { { Parameter::Name, ParamAccess::Read },
        { Parameter::Address, ParamAccess::Read },
        { Parameter::State, ParamAccess::ReadNotify },
        { Parameter::FirmwareMode, ParamAccess::ReadWrite },
        { Parameter::SamplingFrequency, ParamAccess::Read },
        { Parameter::Gain, ParamAccess::Read },
        { Parameter::Offset, ParamAccess::Read }};
}

void BrainbitImpl::setParamChangedCallback(param_changed_callback_t callback) {
    parameterChangedCallback = callback;
}

bool BrainbitImpl::execute(Command command){
    switch (command){
    case Command::StartSignal:
        return execStartSignalCommand();
    case Command::StopSignal:
        return execStopSignalCommand();
    case Command::StartResist:
        return execStartResistCommand();
    case Command::StopResist:
        return execStopResistCommand();
    default:
        throw std::runtime_error("Unsupported command");
    }
}

int BrainbitImpl::batteryChargePercents(){
    return mBatteryPercents;
}

bool BrainbitImpl::isElectrodesAttached(){
    throw std::runtime_error("Device does not support electrodes state request");
}

std::size_t BrainbitImpl::packetsLost() {
    return mPacketCounter.packetsLost();
}

std::size_t BrainbitImpl::packetsReceived() {
    return mPacketCounter.packetsReceived();
}

const BaseBuffer<signal_sample_t> &BrainbitImpl::signalBuffer() const {
    return mSignalBuffer;
}

const BaseBuffer<resp_sample_t> &BrainbitImpl::respirationBuffer() const {
    throw std::runtime_error("Device does not have respiration buffer");
}

const BaseBuffer<MEMS> &BrainbitImpl::memsBuffer() const {
    throw std::runtime_error("Device does not have MEMS buffer");
}

const BaseBuffer<Quaternion> &BrainbitImpl::orientationBuffer() const {
    throw std::runtime_error("Device does not have angle buffer");
}

const BaseBuffer<resistance_sample_t> &BrainbitImpl::resistanceBuffer() const {
    return mResistanceBuffer;
}

void BrainbitImpl::onDataReceived(const ByteBuffer &data){
    if (data.size() != BRAINBIT_PACKET_SIZE){
        LOG_WARN_V("Wrong data packet size: %d, expected: %d", data.size(), BRAINBIT_PACKET_SIZE);
        return;
    }


    auto packetNumber = static_cast<unsigned short>(data[0]) << 3 | static_cast<unsigned short>(data[1]) >> 5;
    auto buttonStateChanged = static_cast<bool>(data[1] & 0x10);
    LOG_TRACE_V("Signal packet received: %d, button state changed: %d", packetNumber, static_cast<int>(buttonStateChanged));

    constexpr static double K = 2.4 / (0xFFFFF * 6);

    int rawData[8];
    rawData[0] = (int)(((unsigned int)(data[1] & 0x0F) << 28) | ((unsigned int)data[2] << 20) | ((unsigned int)data[3] << 12) | ((unsigned int)data[4] << 4));
    rawData[0] /= 2048;

    rawData[1] = (int)(((unsigned int)(data[4] & 0x7F) << 25) | ((unsigned int)data[5] << 17) | ((unsigned int)data[6] << 9) | ((unsigned int)data[7] << 1));
    rawData[1] /= 2048;

    rawData[2] = (int)(((unsigned int)(data[6] & 0x03) << 30) | ((unsigned int)data[7] << 22) | ((unsigned int)data[8] << 14) | ((unsigned int)data[9] << 6));
    rawData[2] /=  2048;

    rawData[3] = (int)(((unsigned int)(data[9] & 0x07) << 27) | ((unsigned int)data[10] << 19) | ((unsigned int)data[11] << 11) | ((unsigned int)data[12] << 3));
    rawData[3] /=  2048;

    rawData[4] = (short)((unsigned short)(data[12] << 8) | (unsigned short)data[13]);
    rawData[4] += rawData[0];
    rawData[5] = (short)((unsigned short)(data[14] << 8) | (unsigned short)data[15]);
    rawData[5] += rawData[1];
    rawData[6] = (short)((unsigned short)(data[16] << 8) | (unsigned short)data[17]);
    rawData[6] += rawData[2];
    rawData[7] = (short)((unsigned short)(data[18] << 8) | (unsigned short)data[19]);
    rawData[7] += rawData[3];

    std::vector<double> signalData(8);
    for (auto i = 0; i < 8; ++i){
        signalData[i] = rawData[i]*K;
    }

    try{
       /* auto packetsLost = mPacketCounter.onNewPacket(packetNumber);
        if (packetsLost > 0){
            LOG_WARN_V("Lost %d packets", packetsLost);
            static constexpr std::size_t samplesInPacket = 8;
            std::vector<double> zeroBuffer(packetsLost * samplesInPacket, 0.0);
            mSignalBuffer.append(zeroBuffer);
            mResistanceBuffer.append(zeroBuffer);
        }
        if (packetsLost < 0)
            return;*/
        if (mBrainbitState == BrainbitCommand::CMD_SIGNAL){
            mSignalBuffer.append(signalData);
            mResistanceBuffer.append(std::vector<double>(8));
        }
        else if (mBrainbitState == BrainbitCommand::CMD_RESIST){
            mResistanceBuffer.append(signalData);
            mSignalBuffer.append(std::vector<double>(8));
        }
    }
    catch (std::exception &e){
        LOG_ERROR_V("Unable to add new packets: %s. Packet number: %zd", e.what(), packetNumber);
    }

}

void BrainbitImpl::onStatusDataReceived(const ByteBuffer &status_data){
    LOG_DEBUG("Status data received");
    BrainbitCommand cmd;
    if (!parseCommand(status_data[0], &cmd)){
        LOG_WARN_V("Command parsing failed for command code %d", status_data[0]);
    }
    parseBattery(status_data);
    parseState(cmd, status_data);
}

void BrainbitImpl::onParameterChanged(Parameter param) {
    if (parameterChangedCallback){
        parameterChangedCallback(param);
    }
}

void BrainbitImpl::parseBattery(const ByteBuffer &status_data){
    LOG_DEBUG("Parsing battery state");
    //isCharging = status_data[STATUS_CHARGE_BYTE_POS] & 0x80; //most significant bit in this byte indicates whether battery is charging or not
    mBatteryPercents = status_data[STATUS_CHARGE_BYTE_POS] & 0x7F; //7 less significant bits indicates charge level in percents
    LOG_DEBUG_V("Battery state: %d", mBatteryPercents);
}

void BrainbitImpl::parseState(BrainbitCommand cmd, const ByteBuffer &status_data){
    LOG_DEBUG_V("Device new state: %d, previous state: %d", static_cast<int>(cmd), static_cast<int>(mBrainbitState));
    if (mBrainbitState != cmd){
        LOG_DEBUG("Processing response");
        mBrainbitState = cmd;
        mRequestHandler->onCommandResponse(cmd, status_data.data()+1, status_data.size()-1);
    }
}

bool BrainbitImpl::execStartSignalCommand(){
    if (mBrainbitState == BrainbitCommand::CMD_SIGNAL){
        LOG_DEBUG("Device is already in signal mode");
        return true;
    }

    auto cmdData = std::make_shared<BrainbitCommandData>(BrainbitCommand::CMD_SIGNAL);
    cmdData->setRequestData({0x00, 0x00, 0x00, 0x00});
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != BrainbitCommandError::NO_ERROR || mBrainbitState != BrainbitCommand::CMD_SIGNAL){
        LOG_ERROR_V("Failed send start receive command. Error code: %d", cmdData->getError());
        return false;
    }
    return true;
}

bool BrainbitImpl::execStopSignalCommand(){
    return stopAll();
}

bool BrainbitImpl::execStartResistCommand(){
    if (mBrainbitState == BrainbitCommand::CMD_RESIST){
        LOG_DEBUG("Device is already in resist mode");
        return true;
    }

    auto cmdData = std::make_shared<BrainbitCommandData>(BrainbitCommand::CMD_RESIST);
    cmdData->setRequestData({0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0xA0});
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != BrainbitCommandError::NO_ERROR || mBrainbitState != BrainbitCommand::CMD_RESIST){
        LOG_ERROR_V("Failed send start resist command. Error code: %d", cmdData->getError());
        return false;
    }
    return true;
}

bool BrainbitImpl::execStopResistCommand(){
    return stopAll();
}

bool BrainbitImpl::stopAll(){
    LOG_DEBUG("Sending STOP command");
    if (mBrainbitState == BrainbitCommand::CMD_STOP){
        LOG_DEBUG("Device already stopped");
        return false;
    }
    auto cmdData = std::make_shared<BrainbitCommandData>(BrainbitCommand::CMD_STOP);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    LOG_DEBUG("Stop all command response received");

    return (cmdData->getError() == BrainbitCommandError::NO_ERROR) && mBrainbitState == BrainbitCommand::CMD_STOP;
}

void BrainbitImpl::sendCommandPacket(std::shared_ptr<BrainbitCommandData> cmdData) {
    auto requestData = cmdData->getRequestData();
    if (requestData.size() > BRAINBIT_MAX_CMD_LENGTH){
        LOG_ERROR("Invalid command data length");
        return;
    }

    LOG_DEBUG_V("Sending command packet for command %d", cmdData->getCommand());

    std::vector<Byte> commandBuffer(requestData.size() + 1);
    commandBuffer[0] = static_cast<Byte>(cmdData->getCommand());
    std::copy(requestData.begin(), requestData.end(), commandBuffer.begin() + 1);

    mBleDevice->sendCommand(commandBuffer);
    LOG_DEBUG("Command been sent");
}

}
