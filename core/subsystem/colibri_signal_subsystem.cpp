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
#include "saturation_cast.h"

/*
ColibriSignalSubsystem::ColibriSignalSubsystem(std::shared_ptr<ColibriDevice> device):
    colibriDevicePtr(device),
    signalReader(std::unique_ptr<SignalBuffer>(new SignalBuffer(saturation_cast<std::size_t>(colibriSamplingFreqToInt(device->sensorParams.samplingFrequency)) *
                 COLIBRI_BUFFER_DURATION))){

    channels.push_back(std::make_shared<Channel>(0, "Signal", signalReader));
}

void ColibriSignalSubsystem::startReceive(){

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::START_ADC_DATA_THROW);
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    if (colibriDevice->getState() != DeviceState::CONNECTED && colibriDevice->getState() != DeviceState::WORKING){
        log->debug("[%s: %s] Unable proceed with command: device disconnected", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    int i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    do {
        log->debug("[%s: %s] Sending start receive request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        colibriDevice->requestHandler->sendRequest(cmdData);
        cmdData->wait();
    }
    while (cmdData->getError() != ColibriCommandError::NO_ERROR && --i);
}

void ColibriSignalSubsystem::stopReceive() {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::STOP_ADC_DATA_THROW);
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    if (colibriDevice->getState() != DeviceState::CONNECTED && colibriDevice->getState() != DeviceState::WORKING){
        log->debug("[%s: %s] Unable proceed with command: device disconnected", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    int i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    do{
        log->debug("[%s: %s] Sending stop receive request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        colibriDevice->requestHandler->sendRequest(cmdData);
        cmdData->wait();
    }
    while (cmdData->getError() != ColibriCommandError::NO_ERROR && --i);
}

void ColibriSignalSubsystem::setSamplingFrequency(int frequency) {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SET_FSAM);
    std::vector<unsigned char> frequencyCode = { static_cast<unsigned char>( colibriSamplingFrequencyFromInt(frequency) ) };
    cmdData->setRequestData(frequencyCode);
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    if (colibriDevice->getState() != DeviceState::CONNECTED && colibriDevice->getState() != DeviceState::WORKING){
        log->debug("[%s: %s] Unable proceed with command: device disconnected", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    int i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    do{
        log->debug("[%s: %s] Sending set sampling frequency request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        colibriDevice->requestHandler->sendRequest(cmdData);
        cmdData->wait();
    }
    while (cmdData->getError() != ColibriCommandError::NO_ERROR && --i);
    while (!colibriDevice->requestSensorParams() && --i);

    if (colibriDevice->sensorParams.samplingFrequency!=colibriSamplingFrequencyFromInt(frequency)) return;

    signalReader.setSamplingFrequency(frequency);
}

int ColibriSignalSubsystem::getSamplingFrequency() {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return 0;
    }

    return colibriSamplingFreqToInt(colibriDevice->sensorParams.samplingFrequency);
}

std::vector<int> ColibriSignalSubsystem::getSupportedFrequencies() const {
    return colibriSupportedFrequencies();
}

void ColibriSignalSubsystem::onDataReceived(std::vector<SignalSample> &samples) {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Received %zd samples", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, samples.size());
    signalReader.onDataReceived(samples);
}

int ColibriSignalSubsystem::getOffset() const {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return 0;
    }

    return colibriDevice->sensorParams.dataOffset;
}

int ColibriSignalSubsystem::getGain() const {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return 0;
    }

    return colibriGainToInt(colibriDevice->sensorParams.PAGain);
}

void ColibriSignalSubsystem::setOffset(int offset) {
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SET_DATA_OFFSET);
    std::vector<unsigned char> offsetBytes = { saturation_cast<unsigned char>(offset) };
    cmdData->setRequestData(offsetBytes);
    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    if (colibriDevice->getState() != DeviceState::CONNECTED && colibriDevice->getState() != DeviceState::WORKING){
        log->debug("[%s: %s] Unable proceed with command: device disconnected", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    int i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    do{
        log->debug("[%s: %s] Sending set offset request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        colibriDevice->requestHandler->sendRequest(cmdData);
        cmdData->wait();
    }
    while (cmdData->getError() != ColibriCommandError::NO_ERROR && --i);

    i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    while (!colibriDevice->requestSensorParams() && --i);

    if (colibriDevice->sensorParams.dataOffset != offset){
        log->error("[%s: %s] Failed setting offset", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    }
}

void ColibriSignalSubsystem::setGain(int intGain) {
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SET_PGA_GAIN);
    auto gain = colibriGainFromInt(intGain);

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice){
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    if (colibriDevice->getState() != DeviceState::CONNECTED && colibriDevice->getState() != DeviceState::WORKING){
        log->debug("[%s: %s] Unable proceed with command: device disconnected", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    std::vector<unsigned char> gainBytes = { static_cast<unsigned char>(gain) };
    cmdData->setRequestData(gainBytes);


    int i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    do{
        log->debug("[%s: %s] Sending set gain request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        colibriDevice->requestHandler->sendRequest(cmdData);
        cmdData->wait();
    }
    while (cmdData->getError() != ColibriCommandError::NO_ERROR && --i);

    i = COLIBRI_CMD_SEND_MAX_ATTEMPTS;
    while (!colibriDevice->requestSensorParams() && --i);

    if (colibriDevice->sensorParams.PAGain != gain) {
        log->error("[%s: %s] Failed setting gain. Read gain: %d, device gain: %d",
                   COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, colibriDevice->sensorParams.PAGain,
                   gain);
    }

}

bool ColibriSignalSubsystem::getElectrodeState() {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    auto colibriDevice = colibriDevicePtr.lock();
    if (!colibriDevice) {
        log->error("[%s: %s] Device been destructed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return false;
    }

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_ELECTRODE_STATE);
    log->debug("[%s: %s] Sending electrode state request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    colibriDevice->requestHandler->sendRequest(cmdData);
    cmdData->wait();

    auto error = cmdData->getError();
    if (error != ColibriCommandError::NO_ERROR) {
        log->warn("[%s: %s] Failed get electrode state. Error code: %d", COLIBRI_DEVICE_CLASSNAME,
                  __FUNCTION__, error);
        return false;
    }
    //electrode state consists of 1 bytes which represent byte
    auto responseLength = cmdData->getResponseLength();

    if (responseLength >= sizeof(unsigned char)) {
        unsigned char electrodeState;
        auto responseData = cmdData->getResponseData();
        std::copy(responseData.begin(), responseData.begin() + sizeof(unsigned char),
                  &electrodeState);

        log->debug("[%s: %s] Electrode state received: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   electrodeState);
        return !electrodeState;
    }

    log->warn("[%s: %s] Failed get electrode state. Response length is zero",
              COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    return false;
}

*/





















