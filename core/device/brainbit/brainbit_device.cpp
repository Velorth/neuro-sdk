/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cmath>
#include "device/brainbit_device.h"
#include "device/brainbit_protocol.h"
#include "signal/channel.h"

using std::pair;
using std::string;
using std::shared_ptr;

/*
BrainbitDevice::BrainbitDevice(std::unique_ptr<BleDevice> bleDevice):
        Device(std::move(bleDevice)),
        deviceState(BrainbitCommand::ERROR),
        batteryLevel(0),
        requestHandler(new BrainbitRequestHandler([this](shared_ptr<BrainbitCommandData> commandData){this->sendCommandPacket(commandData);})){}

void BrainbitDevice::onDataReceived(const std::vector<Byte> &data){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    if (data.size() != BRAINBIT_PACKET_SIZE){
        log->error("[%s: %s] Wrong data packet size: %d, expected: %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__, data.size(), BRAINBIT_PACKET_SIZE);
        return;
    }

    if (deviceState == BrainbitCommand::CMD_SIGNAL){


        auto packetNumber = static_cast<Byte>(data[0]) << 3 | static_cast<Byte>(data[1]) >> 5;
        auto buttonStateChanged = static_cast<bool>(data[1] & 0x10);
        log->trace("[%s: %s] Signal packet received: %d, button state changed: %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__, packetNumber, static_cast<int>(buttonStateChanged));

        constexpr static SignalSample K = 2.4 / (0xFFFFF * 6);

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

        std::vector<SignalSample> signalData(8);
        for (auto i = 0; i < 8; ++i){
            signalData[i] = rawData[i]*K;
        }

        signalSubsystem->onDataReceived(signalData);
    }
}

void BrainbitDevice::onStatusDataReceived(const std::vector<Byte> &status_data){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Status data received", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);

    BrainbitCommand cmd;
    if (!parseCommand(status_data[0], &cmd)){

        log->warn("[%s: %s] Command parsing failed for command code %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__,status_data[0]);
    }
    log->debug("[%s: %s] Parsing battery state", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
    isCharging = status_data[STATUS_CHARGE_BYTE_POS] & 0x80; //most significant bit in this byte indicates whether battery is charging or not
    batteryLevel = status_data[STATUS_CHARGE_BYTE_POS] & 0x7F; //7 less significant bits indicates charge level in percents
    log->debug("[%s: %s] Battery state: %d, is charging: %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__, batteryLevel, static_cast<int>(isCharging));
    notifyBatteryStateChanged(batteryLevel, isCharging);


    log->debug("[%s: %s] Device new state: %d, previous state: %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__, static_cast<int>(cmd), static_cast<int>(deviceState));

    if (deviceState != cmd){
        log->debug("[%s: %s] Processing response", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
        deviceState = cmd;
        requestHandler->onCommandResponse(cmd, status_data.data()+1, status_data.size()-1);
    }
}

bool BrainbitDevice::initialize(){

    deviceFeatures.push_back(DeviceFeature::SIGNAL);    
    mAvailableChannels.push_back(Neuro::ChannelInfo::Battery);
    mAvailableChannels.push_back(Neuro::ChannelInfo(Neuro::ChannelInfo::Type::Signal, "T3"));
    mAvailableChannels.push_back(Neuro::ChannelInfo(Neuro::ChannelInfo::Type::Signal, "T4"));
    mAvailableChannels.push_back(Neuro::ChannelInfo(Neuro::ChannelInfo::Type::Signal, "O1"));
    mAvailableChannels.push_back(Neuro::ChannelInfo(Neuro::ChannelInfo::Type::Signal, "O2"));
    mAvailableChannels.push_back(Neuro::ChannelInfo::Resistance);
    deviceState = BrainbitCommand::ERROR;
    return stopAll();
}

std::weak_ptr<SignalSubsystem> BrainbitDevice::getSignalSubsystem(){

    if (signalSubsystem)
        return std::weak_ptr<SignalSubsystem>(signalSubsystem);

    signalSubsystem = std::make_shared<BrainbitSignalSubsystem>(this);
    return std::weak_ptr<SignalSubsystem>(signalSubsystem);
}

std::weak_ptr<StimulationSubsystem> BrainbitDevice::getStimulationSubsystem(){

    return std::shared_ptr<StimulationSubsystem>();
}

void BrainbitDevice::sendCommandPacket(std::shared_ptr<BrainbitCommandData> cmdData){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto requestData = cmdData->getRequestData();
    if (requestData.size() > BRAINBIT_MAX_CMD_LENGTH){

        log->error("[%s: %s] Invalid command data length", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
        return;
    }

    log->debug("[%s: %s] Sending command packet for command %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__, cmdData->getCommand());

    std::vector<Byte> commandBuffer(requestData.size() + 1);
    commandBuffer[0] = static_cast<Byte>(cmdData->getCommand());
    std::copy(requestData.begin(), requestData.end(), commandBuffer.begin() + 1);

    bleDevice->sendCommand(commandBuffer);
    log->debug("[%s: %s] Command been sent", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
}

bool BrainbitDevice::stopAll(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Sending STOP command", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
    if (deviceState == BrainbitCommand::CMD_STOP){
        log->debug("[%s: %s] Device already stopped", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
        return false;
    }
    auto cmdData = std::make_shared<BrainbitCommandData>(BrainbitCommand::CMD_STOP);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    log->debug("[%s: %s] Stop all command response received", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);

    return (cmdData->getError() == BrainbitCommandError::NO_ERROR) && deviceState == BrainbitCommand::CMD_STOP;
}

void BrainbitDevice::sendStartReceive(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    if (deviceState == BrainbitCommand::CMD_SIGNAL){
        log->debug("[%s: %s] Device is already in signal mode", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__);
        return;
    }

    auto cmdData = std::make_shared<BrainbitCommandData>(BrainbitCommand::CMD_SIGNAL);
    cmdData->setRequestData({0x00, 0x00, 0x00, 0x00});
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != BrainbitCommandError::NO_ERROR || deviceState != BrainbitCommand::CMD_SIGNAL){
        log->error("[%s: %s] Failed send start receive command. Error code: %d", BRAINBIT_SOURCE_CLASSNAME, __FUNCTION__, cmdData->getError());
    }
}

void BrainbitDevice::sendStartResist(){


}

int BrainbitDevice::getBatteryLevel() const{

    return batteryLevel;
}

bool BrainbitDevice::isBatteryCharging() const
{
    return isCharging;
}

*/






