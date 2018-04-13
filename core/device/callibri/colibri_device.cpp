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

#include "device/colibri_device.h"
#include "logger.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <device/callibri_protocol.h>

/*
ColibriDevice::ColibriDevice(std::unique_ptr<BleDevice> bleDevice):
        NeuroDevice(std::move(bleDevice)),
        requestHandler(std::make_shared<ColibriRequestHandler>([this](std::shared_ptr<ColibriCommandData> commandData){this->sendCommandPacket(commandData);})),
        sensorAddress(0){

}

ColibriDevice::~ColibriDevice(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Colibri device destructor", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    std::unique_lock<std::mutex> batteryStateLock(batteryStateMutex);
    batteryStateCondition.notify_all();
    batteryStateLock.unlock();
    if (batteryStateRequestThread.joinable()){
        log->debug("[%s: %s] Join", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        batteryStateRequestThread.join();
    }
    else{
        log->debug("[%s: %s] Is not joinable", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    }
    log->debug("[%s: %s] Colibri device destructor EXIT", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
}

ColibriMode ColibriDevice::sendEcho()
{
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::ECHO);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] ECHO received", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

    if (cmdData->getError()!=ColibriCommandError::NO_ERROR) return ColibriMode::ERROR;

    auto responseLength = cmdData->getResponseLength();
    //TODO Add additional request in case of error responses
    if (responseLength >= 1)
    {
        auto responseData = cmdData->getResponseData();
        //checks most significant bit. if it is equal to 1,
        // than sensor in bootloader mode, if msb is 0, than in app mode
        return (responseData[0] & 0x80) ? ColibriMode::BOOTLOADER : ColibriMode::APPLICATION;
    }
    return ColibriMode::ERROR;
}

long ColibriDevice::requestDeviceAddress()
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_ADDR);
    log->debug("[%s: %s] Sending address request...", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();    

    //since sensor address consists of 3 bytes, we won't get
    // negative long from it even on LP32 platforms,
    //so we could use negative values to indicate errors
    if (cmdData->getError()!=ColibriCommandError::NO_ERROR) {
        log->debug("[%s: %s] Failed to receive address", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return -1;
    }

    auto responseLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, responseLength);
    if (responseLength >= COLIBRI_ADDRESS_LENGTH)
    {
        auto responseData = cmdData->getResponseData();
        ColibriLong address;
        address.value = 0;
        std::copy(responseData.begin(), responseData.begin()+COLIBRI_ADDRESS_LENGTH, address.bytes);
        log->debug("[%s: %s] Address is %ld", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, address.value);
        return address.value;
    }

    //responseLength is zero
    return -2;
}

bool ColibriDevice::activateApplication(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::ACTIVATE_APP);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
    log->debug("[%s: %s] Activate application response received", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    
    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}

int ColibriDevice::requestBatteryVoltage(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto sharedThis = derivedPtr<ColibriDevice>();
    if (!sharedThis) {
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return -1;
    }
    if (!sharedThis->isInitialized)
        return false;
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_BATTERY_V);
    log->debug("[%s: %s] Sending battery level request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    sharedThis->requestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto error = cmdData->getError();
    if (error != ColibriCommandError::NO_ERROR) {
        log->warn("[%s: %s] Failed get battery voltage. Error code: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, error);
        return -1;
    }

    //battery voltage value consists of 2 bytes which represent unsigned short integer
    auto responseLength = cmdData->getResponseLength();

    if (responseLength >= sizeof(ColibriUShort::value))
    {
        ColibriUShort batteryVoltage;
        auto responseData = cmdData->getResponseData();
        std::copy(responseData.begin(), responseData.begin() + sizeof(ColibriUShort::value), batteryVoltage.bytes);

        log->debug("[%s: %s] Battery voltage received: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, batteryVoltage.value);
        return batteryVoltage.value;
    }

    log->warn("[%s: %s] Failed get battery voltage. Response length is zero", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    return -2;
}

bool ColibriDevice::requestSensorParams(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_SENSOR_PARAM);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
    log->debug("[%s: %s] Sensor params received", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    
    if (cmdData->getError() != ColibriCommandError::NO_ERROR) return false;

    auto responseDataLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, responseDataLength);
    if (responseDataLength >= COLIBRI_SENSOR_PARAMS_DATA_LENGTH)
    {
        auto responseData = cmdData->getResponseData();
        log->debug("[%s: %s] Getting features", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        auto featuresFlags = responseData[0];
        if (featuresFlags&0x01)
        {
            deviceFeatures.push_back(DeviceFeature::SIGNAL);
            mAvailableChannels.push_back(Neuro::ChannelInfo::Signal);
            mAvailableChannels.push_back(Neuro::ChannelInfo::ElectrodesState);
            mAvailableChannels.push_back(Neuro::ChannelInfo::ConnectionStats);
            log->debug("[%s: %s] Has signal feature", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        }
        if (featuresFlags&0x02)
        {
            deviceFeatures.push_back(DeviceFeature::MEMS);
            mAvailableChannels.push_back(Neuro::ChannelInfo::MEMS);
            mAvailableChannels.push_back(Neuro::ChannelInfo::Angle);
            mAvailableChannels.push_back(Neuro::ChannelInfo::ConnectionStats);
            log->debug("[%s: %s] Has MEMS feature", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        }
        if (featuresFlags&0x04)
        {
            deviceFeatures.push_back(DeviceFeature::STIMUL);
            log->debug("[%s: %s] Has stimulation feature", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        }
        if (featuresFlags&0x08)
        {
            deviceFeatures.push_back(DeviceFeature::SPYRO);
            mAvailableChannels.push_back(Neuro::ChannelInfo::Respiration);
            mAvailableChannels.push_back(Neuro::ChannelInfo::ConnectionStats);
            log->debug("[%s: %s] Has spyrometry feature", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        }
        sensorParams.samplingFrequency = static_cast<ColibriSamplingFreq>(responseData[1]); //this is unsafe conversion because we might get wrong value in response
        log->debug("[%s: %s] Sampling frequency: %d Hz", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, colibriSamplingFreqToInt(sensorParams.samplingFrequency));

        sensorParams.adcMode = static_cast<ColibriAdcInputMode >(responseData[COLIBRI_ADC_INPUT_MODE_BYTE_POS]); //this is unsafe conversion because we might get wrong value in response
        log->debug("[%s: %s] ADC input mode: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   static_cast<int>(sensorParams.adcMode));

        sensorParams.switchState = static_cast<ColibriExtSwitchState>(responseData[COLIBRI_EXT_SWITCH_STATE_BYTE_POS]); //this is unsafe conversion because we might get wrong value in response
        log->debug("[%s: %s] External switch state: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   static_cast<int>(sensorParams.switchState));
        
        sensorParams.highPassFilterState = responseData[COLIBRI_FILTER_STATE_BYTE_POS];
        log->debug("[%s: %s] Hign pass filter %s", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, sensorParams.highPassFilterState?"enabled":"disabled");
        
        sensorParams.PAGain = static_cast<ColibriGain>(responseData[COLIBRI_GAIN_BYTE_POS]);
        log->debug("[%s: %s] Signal gain: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, colibriGainToInt(sensorParams.PAGain));

        sensorParams.dataOffset = responseData[COLIBRI_DATA_OFFSET_BYTE_POS];
        log->debug("[%s: %s] Data offset: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, sensorParams.dataOffset);
        return true;
    }
    return false;
}

bool ColibriDevice::switchAdcState(ColibriAdcInputMode adcMode) {
    if (!isInitialized)
        return false;

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SWITCH_ADC_INP);
    std::vector<unsigned char> adcState = { static_cast<unsigned char>(adcMode) };
    cmdData->setRequestData(adcState);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}


bool ColibriDevice::changeExternalSwitchState(ColibriExtSwitchState switchState) {
    if (!isInitialized)
        return false;

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SWITCH_EXT_COM_INPUTS);
    std::vector<unsigned char> adcState = { static_cast<unsigned char>(switchState) };
    cmdData->setRequestData(adcState);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}


void ColibriDevice::batteryStateRequestFunc() {

    std::unique_lock<std::mutex> batteryStateLock(batteryStateMutex);
    do {
        std::weak_ptr<ColibriDevice> weakThis = derivedPtr<ColibriDevice>();
        if (auto sharedThis = weakThis.lock()) {
            if (sharedThis->deviceState != DeviceState::CONNECTED &&
                sharedThis->deviceState != DeviceState::WORKING) {
                continue;
            }

            auto voltage = sharedThis->requestBatteryVoltage();
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
            if (sharedThis->batteryState.load() != batteryLevel) {
                batteryState.store(batteryLevel);
                notifyBatteryStateChanged(batteryLevel, false);
            }
        }
        else {
            LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Device been destructed",
                                                             COLIBRI_DEVICE_CLASSNAME,
                                                             __FUNCTION__);
            return;
        }
    } while (batteryStateCondition.wait_for(batteryStateLock, std::chrono::seconds(
            COLIBRI_BATTERY_STATE_REQUEST_DELAY_SECONDS)) == std::cv_status::timeout);
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Battery thread EXIT", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
}

bool ColibriDevice::switchFilterState(bool turnOn){

    if (!isInitialized)
        return false;

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SWITCH_FILTER_STATE);
    std::vector<unsigned char> filterState = { static_cast<unsigned char>(turnOn) };
    cmdData->setRequestData(filterState);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}


void ColibriDevice::onDataReceived(const std::vector<Byte> &data){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto sharedThis = derivedPtr<ColibriDevice>();
    if (!sharedThis) {
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }
    log->trace("[%s: %s] Data received. Length: %zd", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, data.size());
    if (data.size() < COLIBRI_PACKET_SIZE) return;
    //we should check if we received command or data.
    //here we parse packet to get base information from beacon
    ColibriUShort marker;
    marker.bytes[0] = data[0];
    marker.bytes[1] = data[1];

    log->trace("[%s: %s] Marker: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, marker.value);

    //Signal data received
    if (marker.value <= COLIBRI_DATA_MAX_PACKET_NMB)
    {
        if (sharedThis->isInitialized)
        {
            log->trace("[%s: %s] Signal received. Packet number: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, marker.value);
            onSignalReceived(marker.value, data.data()+COLIBRI_SIGNAL_DATA_START_POS, data.size()-COLIBRI_SIGNAL_DATA_START_POS);
        }
    }
    //Colibri command response received
    else if (marker.value == COLIBRI_COMMAND_MARKER)
    {
        log->debug("[%s: %s] Command marker", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        char checksum = 0;
        for (auto headerByte = data.data();
             headerByte != data.data() + COLIBRI_CMD_HDR_DATA_START_POS; checksum += *headerByte++);

        log->debug("[%s: %s] Checksum: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,checksum);

        //if checksum is not 0 discard packet
        if (checksum) {
            log->warn("[%s: %s] Checksum isn't equals to zero. Discarding packet.", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
            return;
        }

        ColibriLong hostAddress;
        hostAddress.value = 0;
        std::copy(data.data() + COLIBRI_HEADER_ADDR_POSITION,
             data.data() + COLIBRI_HEADER_ADDR_POSITION + COLIBRI_ADDRESS_LENGTH, hostAddress.bytes);

        //if host address is not valid discard packet
        if (hostAddress.value!=COLIBRI_HOST_ADDRESS){
            log->error("[%s: %s] Host address is not valid: %ld. Must be %ld", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, hostAddress.value, COLIBRI_HOST_ADDRESS);
            return;
        }

        sharedThis->onCommandPacketReceived(data.data(), data.size());
    }
    //Spirometry data received
    else if (marker.value == COLIBRI_SPIRO_DATA_MARKER)
    {
        //onSpyrometryDataReceived(data, length);
    }
    //MEMS data received
    else if (marker.value == COLIBRI_MEMS_DATA_MARKER)
    {
        //onMemsDataReceived(data,length);
    }
}


void ColibriDevice::onSignalReceived(int, const unsigned char *data, size_t length)
{
    //TODO add sequence control for packets
    auto log = LoggerFactory::getCurrentPlatformLogger();
    if (signalSubsystem == nullptr)
    {
        log->warn("[%s: %s] Signal subsystem is not initialized. Skip packet", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    std::vector<SignalSample> samples;
    for (auto sample = data; sample<data+length; sample+=sizeof(ColibriShort::bytes)) {
        ColibriShort shortSample;
        shortSample.bytes[0] = *sample;
        shortSample.bytes[1] = *(sample + 1);

        auto adcValue = shortSample.value;
        SignalSample sampleValue =
                ((SignalSample) COLIBRI_ADC_REF_VOLTAGE / COLIBRI_ADC_CAPACITY) * adcValue /
                colibriGainToInt(sensorParams.PAGain);

        samples.push_back(sampleValue);
    }

    signalSubsystem->onDataReceived(samples);//buffer
}

void ColibriDevice::onCommandPacketReceived(const unsigned char *const command_packet, size_t length)
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Command response received", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    ColibriCommand command;
    if (!parseCommand(command_packet[COLIBRI_HEADER_CMD_POSITION], &command))
    {
        log->debug("[%s: %s] Command parsing failed for command code %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,command_packet[COLIBRI_HEADER_CMD_POSITION]);
        return;
    }

    log->debug("[%s: %s] Processing response", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->onCommandResponse(command,
                                       command_packet + COLIBRI_CMD_HDR_DATA_START_POS,
                                       length - COLIBRI_CMD_HDR_DATA_START_POS);
}

void ColibriDevice::sendCommandPacket(std::shared_ptr<ColibriCommandData> commandData)
{
    //compose command packet and send
    std::vector<Byte> packet(COLIBRI_PACKET_SIZE);

    ColibriUShort commandMarker;
    commandMarker.value = COLIBRI_COMMAND_MARKER;

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto sharedThis = derivedPtr<ColibriDevice>();
    if (!sharedThis) {
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }
    log->debug("[%s: %s] Sending command packet for command %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, commandData->getCommand());
    
    packet[0] = commandMarker.bytes[0];
    packet[1] = commandMarker.bytes[1];
    packet[COLIBRI_HEADER_CMD_POSITION] = static_cast<unsigned char>(commandData->getCommand());

    ColibriLong address;
    address.value = sharedThis->sensorAddress;
    packet[COLIBRI_HEADER_ADDR_POSITION] = address.bytes[0];
    packet[COLIBRI_HEADER_ADDR_POSITION + 1] = address.bytes[1];
    packet[COLIBRI_HEADER_ADDR_POSITION + 2] = address.bytes[2];

    unsigned char checksum = 0;
    for (auto packetByte = &packet[0];
         packetByte != &packet[0] + COLIBRI_HEADER_CS_POSITION; checksum -= *packetByte++);
    packet[COLIBRI_HEADER_CS_POSITION] = checksum;

    if (auto dataLength = commandData->getRequestLength())
    {
        //data length exceeds packet size
        //discard this packet
        if (COLIBRI_CMD_HDR_DATA_START_POS + dataLength > COLIBRI_PACKET_SIZE)
            return;

        auto requestData = commandData->getRequestData();
        if (requestData.size() > COLIBRI_PACKET_SIZE + COLIBRI_CMD_HDR_DATA_START_POS)
        {
            log = LoggerFactory::getCurrentPlatformLogger();
            log->warn("[%s: %s] Packet data array too big!", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
            return;
        }

        std::copy(requestData.begin(), requestData.end(), packet.begin()+COLIBRI_CMD_HDR_DATA_START_POS);
    }

    sharedThis->bleDevice->sendCommand(packet);
    log->debug("[%s: %s] Command been sent", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
}

bool ColibriDevice::initialize()
{
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto sharedThis = derivedPtr<ColibriDevice>();
    if (!sharedThis) {
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return false;
    }
    log->debug("[%s: %s] Start init", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

    if(sharedThis->isInitialized){
        log->warn("[%s: %s] Already initialized. Skip.", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return true;
    }

    sharedThis->sensorAddress = 0;
    int attemptsAddrLeft = 3;
    long address;
    while ((address = sharedThis->requestDeviceAddress()) < 0 && attemptsAddrLeft--);
    if (address < 0)
        return false;

    sharedThis->sensorAddress = address;

    log->debug("[%s: %s] Address: %ld", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, sharedThis->sensorAddress);

    int attemptsEchoLeft = 3;

    ColibriMode deviceMode = ColibriMode::ERROR;
    do {
        deviceMode = sharedThis->sendEcho();
    }
    while(deviceMode == ColibriMode::ERROR && attemptsEchoLeft--);
    if (deviceMode == ColibriMode::ERROR)
        return false;

    log->debug("[%s: %s] Echo received", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

    if (deviceMode == ColibriMode::BOOTLOADER)
    {
        log->debug("[%s: %s] Bootloader", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        sharedThis->activateApplication();
    }

    log->debug("[%s: %s] Requesting params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    //requesting params from sensor
    isInitialized = sharedThis->requestSensorParams();
    if (isInitialized) {
        log->debug("[%s: %s] Params received", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        if (!batteryStateRequestThread.joinable()) {
            batteryStateRequestThread = std::thread(&ColibriDevice::batteryStateRequestFunc, this);            
            mAvailableChannels.push_back(Neuro::ChannelInfo::Battery);
        }
    }
    else log->warn("[%s: %s] Failed receive params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    return isInitialized;
}

int ColibriDevice::getBatteryLevel() const {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto sharedThis = derivedPtr<ColibriDevice>();
    if (!sharedThis) {
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return 0;
    }
    return batteryState.load();
}

void ColibriDevice::createSignalSubsystem()
{
    if (signalSubsystem) return;

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Creating subsystem for signal feature", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

    if (std::find(deviceFeatures.begin(), deviceFeatures.end(), DeviceFeature::SIGNAL) ==
        deviceFeatures.end()) {
        log->error("[%s: %s] Feature SIGNAL is not supported in this device", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    //if filter is not turned off, trying to enable it
    while (!sensorParams.highPassFilterState)
    {
        if (!this->switchFilterState(true))
        {
            log->warn("[%s: %s] Unable to switch filter state. Continue without firmware filter", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
            break;
        }
        if (!this->requestSensorParams())
        {
            log->error("[%s: %s] Unable to retrieve device parameters. Continue with unknown device configuration", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
            break;
        }
    }
    while (sensorParams.adcMode != ColibriAdcInputMode::RESIST){
        this->switchAdcState(ColibriAdcInputMode::RESIST);
        this->requestSensorParams();
    }
    while (sensorParams.switchState != ColibriExtSwitchState::RESPUSB_MIOELECTRODES){
        this->changeExternalSwitchState(ColibriExtSwitchState::RESPUSB_MIOELECTRODES);
        this->requestSensorParams();
    }
    signalSubsystem = std::make_shared<ColibriSignalSubsystem>(derivedPtr<ColibriDevice>());
}


void ColibriDevice::createStimulationSubsystem() {

    if (stimulationSubsystem) return;

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Creating subsystem for stimul feature", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

    if (std::find(deviceFeatures.begin(), deviceFeatures.end(), DeviceFeature::STIMUL) ==
        deviceFeatures.end())
        return;

    stimulationSubsystem = std::make_shared<ColibriStimulationSubsystem>(requestHandler);
}

std::weak_ptr<SignalSubsystem> ColibriDevice::getSignalSubsystem() {
    if (!signalSubsystem) createSignalSubsystem();
    return signalSubsystem;
}

std::weak_ptr<StimulationSubsystem> ColibriDevice::getStimulationSubsystem() {
    if (!stimulationSubsystem) createStimulationSubsystem();
    return stimulationSubsystem;
}

bool ColibriDevice::isBatteryCharging() const {
    return false;
}


*/
