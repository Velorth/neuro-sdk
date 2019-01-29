#include "channels/info/channel_info.h"
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
                                                             this->onParameterChanged(param);
                                                         },
														 mSetter),
               std::make_unique<BrainbitParameterWriter>()),
    mRequestHandler(std::make_unique<BrainbitRequestHandler>(
                        [=](std::shared_ptr<BrainbitCommandData> cmd){sendCommandPacket(cmd);})),
    mBatteryNotificationLoop(&BrainbitImpl::notifyBatteryState, std::chrono::seconds(5), this){
    initChannels();
}

std::vector<ChannelInfo> BrainbitImpl::channels() const {
    return mChannels;
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
        { Parameter::Offset, ParamAccess::Read },
		{ Parameter::FirmwareVersion, ParamAccess::Read } };
}

ListenerPtr<void, Parameter> BrainbitImpl::setParamChangedCallback(param_changed_callback_t callback) {
    return mParameterChangedNotifier.addListener(callback);
}

bool BrainbitImpl::execute(Command command){
    switch (command){
    case Command::StartSignal:
        return execStartSignalCommand();
    case Command::StopSignal:
        return execStopSignalCommand();
    case Command::StartResist:
        return startResist();
    case Command::StopResist:
        return execStopResistCommand();
    default:
        throw std::runtime_error("Unsupported command");
    }
}

ListenerPtr<void, const int &>
BrainbitImpl::subscribeBatteryDataReceived(std::function<void(const int &)> callback, ChannelInfo) {
	return mBatteryNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<signal_sample_t> &>
BrainbitImpl::subscribeSignalDataReceived(std::function<void(const std::vector<signal_sample_t> &)> callback, ChannelInfo info) {
	if (info.getType() != ChannelInfo::Type::Signal) {
        throw std::runtime_error("Wrong ChannelInfo Type value");
    }
	auto channels = this->channels();
	if (std::find(channels.begin(), channels.end(), info) == channels.end()){
	    throw std::runtime_error(std::string("Device does not have channel ")+
                                 info.getName()+
                                 std::string(" with index ")+
                                 std::to_string(info.getIndex()));
	}
	if (mSignalNotifierMap.find(info.getIndex()) != mSignalNotifierMap.end()) {
		return mSignalNotifierMap[info.getIndex()].addListener(callback);
	}
	throw std::runtime_error("Unable to subscribe signal data notifications");
}

ListenerPtr<void, const std::vector<resistance_sample_t> &>
BrainbitImpl::subscribeResistanceDataReceived(std::function<void(const std::vector<resistance_sample_t> &)> callback, ChannelInfo info) {
    if (info.getType() != ChannelInfo::Type::Resistance) {
        throw std::runtime_error("Wrong ChannelInfo Type value");
    }
	auto channels = this->channels();
    if (std::find(channels.begin(), channels.end(), info) == channels.end()){
        throw std::runtime_error(std::string("Device does not have channel ")+
                                 info.getName()+
                                 std::string(" with index ")+
                                 std::to_string(info.getIndex()));
    }
	if (mResistanceNotifierMap.find(info.getIndex()) != mResistanceNotifierMap.end()) {
		return mResistanceNotifierMap[info.getIndex()].addListener(callback);
	}
	throw std::runtime_error("Unable to subscribe resistance data notifications");
}

ListenerPtr<void, const std::vector<MEMS> &>
BrainbitImpl::subscribeMEMSDataReceived(std::function<void(const std::vector<MEMS> &)> callback, ChannelInfo) {
	throw std::runtime_error("Unable to subscribe MEMES data notifications");
}

ListenerPtr<void, const std::vector<Quaternion> &>
BrainbitImpl::subscribeOrientationDataReceived(std::function<void(const std::vector<Quaternion> &)> callback, ChannelInfo) {
	throw std::runtime_error("Unable to subscribe orientation data notifications");
}

ListenerPtr<void, const std::vector<double> &>
BrainbitImpl::subscribeRespirationDataReceived(std::function<void(const std::vector<double> &)> callback, ChannelInfo) {
	throw std::runtime_error("Unable to subscribe respiration data notifications");
}

ListenerPtr<void, const int &>
BrainbitImpl::subscribeConnectionStatsDataReceived(std::function<void(const int &)> callback, ChannelInfo) {
	return mConnectionStatsNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<int> &>
BrainbitImpl::subscribePedometerDataReceived(std::function<void(const std::vector<int> &)> callback, ChannelInfo) {
	throw std::runtime_error("Unable to subscribe pedometer data notifications");
}

ListenerPtr<void, const ElectrodeState &>
BrainbitImpl::subscribeElectrodesDataReceived(std::function<void(const ElectrodeState &)> callback, ChannelInfo) {
	throw std::runtime_error("Unable to subscribe electrodes state data notifications");
}

void BrainbitImpl::notifyBatteryState(){
    mBatteryNotifier.notifyAll(mBatteryPercents);
}

void BrainbitImpl::initChannels(){
    auto t3Info = ChannelInfo(ChannelInfo::Type::Signal, "T3", 1);
    auto t4Info = ChannelInfo(ChannelInfo::Type::Signal, "T4", 2);
    auto o1Info = ChannelInfo(ChannelInfo::Type::Signal, "O1", 0);
    auto o2Info = ChannelInfo(ChannelInfo::Type::Signal, "O2", 3);
    auto t3ResistInfo = ChannelInfo(ChannelInfo::Type::Resistance, "T3", 1);
    auto t4ResistInfo = ChannelInfo(ChannelInfo::Type::Resistance, "T4", 2);
    auto o1ResistInfo = ChannelInfo(ChannelInfo::Type::Resistance, "O1", 0);
    auto o2ResistInfo = ChannelInfo(ChannelInfo::Type::Resistance, "O2", 3);
    mChannels = { ChannelInfo::Battery(),
              ChannelInfo::ConnectionStats(),
              t3Info,
              t4Info,
              o1Info,
              o2Info,
              t3ResistInfo,
              t4ResistInfo,
              o1ResistInfo,
              o2ResistInfo };

    std::string name(class_name);

    mSignalNotifierMap.emplace(std::piecewise_construct,
                               std::forward_as_tuple(t3Info.getIndex()),
                               std::forward_as_tuple(name));
    mSignalNotifierMap.emplace(std::piecewise_construct,
                               std::forward_as_tuple(t4Info.getIndex()),
                               std::forward_as_tuple(name));
    mSignalNotifierMap.emplace(std::piecewise_construct,
                               std::forward_as_tuple(o1Info.getIndex()),
                               std::forward_as_tuple(name));
    mSignalNotifierMap.emplace(std::piecewise_construct,
                               std::forward_as_tuple(o2Info.getIndex()),
                               std::forward_as_tuple(name));

    mResistanceNotifierMap.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(t3ResistInfo.getIndex()),
                                   std::forward_as_tuple(name));
    mResistanceNotifierMap.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(t4ResistInfo.getIndex()),
                                   std::forward_as_tuple(name));
    mResistanceNotifierMap.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(o1ResistInfo.getIndex()),
                                   std::forward_as_tuple(name));
    mResistanceNotifierMap.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(o2ResistInfo.getIndex()),
                                   std::forward_as_tuple(name));
}

void BrainbitImpl::onDataReceived(const ByteBuffer &data){
    if (data.size() != BRAINBIT_PACKET_SIZE){
        LOG_WARN_V("Wrong data packet size: %d, expected: %d", data.size(), BRAINBIT_PACKET_SIZE);
        return;
    }    
    parseSignalData(data);
}

void BrainbitImpl::onStatusDataReceived(const ByteBuffer &status_data){
    LOG_DEBUG("Status data received");
    BrainbitCommand cmd;
    if (!parseCommand(status_data[0], &cmd)){
        LOG_WARN_V("Command parsing failed for command code %d", status_data[0]);
    }
    parseBattery(status_data);
    parseState(cmd, status_data);
	parseVersion(status_data);
}

void BrainbitImpl::onParameterChanged(Parameter param) {
	mParameterChangedNotifier.notifyAll(param);
}

void BrainbitImpl::parseBattery(const ByteBuffer &status_data) {
    //isCharging = status_data[STATUS_CHARGE_BYTE_POS] & 0x80;
    mBatteryPercents = status_data[STATUS_CHARGE_BYTE_POS] & 0x7F;
}

void BrainbitImpl::parseState(BrainbitCommand cmd, const ByteBuffer &status_data){
    LOG_DEBUG_V("Device new state: %d, previous state: %d", static_cast<int>(cmd), static_cast<int>(mBrainbitState));
//    if (mBrainbitState != cmd){
 //       LOG_DEBUG("Processing response");
        mBrainbitState = cmd;
        mRequestHandler->onCommandResponse(cmd, status_data.data()+1, status_data.size()-1);
  //  }
}

void BrainbitImpl::parseVersion(const ByteBuffer &status_data) {
	LOG_DEBUG("Parsing firmware version");
	mSetter.FirmwareVersion = FirmwareVersion{ status_data[STATUS_VERSION_BYTE_POS], 0 };
	LOG_DEBUG_V("Firmware version: %d", mSetter.FirmwareVersion);
}

void BrainbitImpl::parseSignalData(const ByteBuffer &data){
    auto packetNumber = static_cast<unsigned short>(data[0]) << 3 | static_cast<unsigned short>(data[1]) >> 5;
    auto buttonStateChanged = static_cast<bool>(data[1] & 0x10);

    constexpr static double K = 2.4 / (0xFFFFF * 6);

    int rawData[8];
    rawData[0] = (int)(((unsigned int)(data[1] & 0x0F) << 28) | ((unsigned int)data[2] << 20) | ((unsigned int)data[3] << 12) | ((unsigned int)data[4] << 4));
    rawData[0] /= 2048;

    rawData[1] = (int)(((unsigned int)(data[4] & 0x7F) << 25) | ((unsigned int)data[5] << 17) | ((unsigned int)data[6] << 9) | ((unsigned int)data[7] << 1));
    rawData[1] /= 2048;

    rawData[2] = (int)(((unsigned int)(data[6] & 0x03) << 30) | ((unsigned int)data[7] << 22) | ((unsigned int)data[8] << 14) | ((unsigned int)data[9] << 6));
    rawData[2] /=  2048;

    rawData[3] = (int)(((unsigned int)(data[9] & 0x1F) << 27) | ((unsigned int)data[10] << 19) | ((unsigned int)data[11] << 11));
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
        if (mBrainbitState == BrainbitCommand::CMD_SIGNAL){
            onSignalReceived(signalData);
        }
        else if (mBrainbitState == BrainbitCommand::CMD_RESIST){
            onResistanceReceived(signalData);
        }
    }
    catch (std::exception &e){
        LOG_ERROR_V("Unable to add new packets: %s. Packet number: %zd", e.what(), packetNumber);
    }
}

void BrainbitImpl::onSignalReceived(const std::vector<signal_sample_t> &data){
    for (auto& info : mChannels){
        if (info.getType() == ChannelInfo::Type::Signal){
            std::vector<signal_sample_t> channelSamples{data[info.getIndex()], data[info.getIndex()+4]};
            mSignalNotifierMap[info.getIndex()].notifyAll(channelSamples);
        }
    }
}

void BrainbitImpl::onResistanceReceived(const std::vector<resistance_sample_t> &data){
    onSignalReceived(std::vector<signal_sample_t>(8));
    if (std::abs(data[mCurrentResistChannel]) == 0.0 || std::abs(data[mCurrentResistChannel+4]) == 0.0 ){
        return;
    }
    mResistBuffer.push_back(data[mCurrentResistChannel]);
    mResistBuffer.push_back(data[mCurrentResistChannel+4]);
    if (mResistBuffer.size() >= 100) {
        double res = 0;
        for (std::size_t r = 20; r < mResistBuffer.size(); r++) {
            if (mResistBuffer[r] >= std::abs(0.4)) {
                onResistanceCalculated(std::numeric_limits<double>::infinity());
                return;
            }
            res += mResistBuffer[r];
        }

        double average = res / (mResistBuffer.size() - 20);
        res = 0;
        for (std::size_t r = 20; r < mResistBuffer.size(); r++) {
            res += std::abs(mResistBuffer[r] - average);
        }
        res = res / (mResistBuffer.size() - 20) / 24e-9;
        res = std::abs(res / 2);
        onResistanceCalculated(res);
    }
}


void BrainbitImpl::onResistanceCalculated(const resistance_sample_t value) {
    mResistBuffer.clear();
    mResistanceNotifierMap[mCurrentResistChannel].notifyAll({value});
    mCurrentResistChannel =  (mCurrentResistChannel + 1) % 4;
    execStartResistCommand(mCurrentResistChannel);
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

bool BrainbitImpl::startResist(){
    if (mBrainbitState == BrainbitCommand::CMD_RESIST){
        LOG_DEBUG("Device is already in resist mode");
        return true;
    }

    mCurrentResistChannel = 0;
    if (!execStartResistCommand(mCurrentResistChannel) || mBrainbitState != BrainbitCommand::CMD_RESIST){
        LOG_ERROR("Failed send start resist command.");
        return false;
    }
    return true;
}

bool BrainbitImpl::execStartResistCommand(std::size_t channel_index){
    auto cmdData = std::make_shared<BrainbitCommandData>(BrainbitCommand::CMD_RESIST);
    std::vector<Byte> requestData(7);
    for (std::size_t i = 0; i < 4; ++i){
        if (i == channel_index){
            requestData[i] = 0b01100000;
        }
        else{
            requestData[i] = 0b10010001;
        }
    }
    requestData[4] = static_cast<Byte>(1) << channel_index;
    requestData[5] = requestData[4];
    cmdData->setRequestData(requestData);
    mRequestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == BrainbitCommandError::NO_ERROR;
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
    mResistBuffer.clear();
    mCurrentResistChannel = 0;

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
