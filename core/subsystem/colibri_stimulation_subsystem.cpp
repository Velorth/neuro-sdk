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
#include "device/callibri_protocol.h"
#include "logger.h"

/*
ColibriStimulationSubsystem::ColibriStimulationSubsystem(std::weak_ptr<ColibriRequestHandler> rh):requestHandlerPtr(rh) {

    auto log = LoggerFactory::getCurrentPlatformLogger();
    if (!this->sendGetStimulParams()) {
        log->error("[%s: %s] Unable to get stimulation params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        throw new UnsupportedFeatureException("Unable to retrieve stimulation params");
    }

    if (!this->sendGetShParams()) {
        log->error("[%s: %s]  Unable to get step helper params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        throw new UnsupportedFeatureException("Unable to retrieve step helper params");
    }
}

ColibriStimulationSubsystem::~ColibriStimulationSubsystem() {

}

void ColibriStimulationSubsystem::stimulStart() {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::START_STIM);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler) {
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME,
                   __FUNCTION__);
        return;
    }


    log->debug("[%s: %s] Sending start stimul request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
}

void ColibriStimulationSubsystem::stimulStop() {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::STOP_STIM);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler) {
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME,
                   __FUNCTION__);
        return;
    }

    log->debug("[%s: %s] Sending stop stimul request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
}

bool ColibriStimulationSubsystem::getStimulatorIsActive() {
    return sendGetState().stimulatorIsActive;;
}

void ColibriStimulationSubsystem::setStimulParams(int current, int pulse_duration, int frequency, int stimulus_duration) {

    stimulParams.stimPulseAmpl = current > std::numeric_limits<unsigned char>::max() ? std::numeric_limits<unsigned char>::max() :   static_cast<unsigned char>(current);
    stimulParams.stimPulseDur = pulse_duration > std::numeric_limits<unsigned char>::max() ? std::numeric_limits<unsigned char>::max() :   static_cast<unsigned char>(pulse_duration);
    stimulParams.stimPulseFreq = frequency > std::numeric_limits<unsigned char>::max() ? std::numeric_limits<unsigned char>::max() :   static_cast<unsigned char>(frequency);
    stimulParams.stimDuration.value = stimulus_duration > std::numeric_limits<short>::max() ? std::numeric_limits<short>::max() : static_cast<short>(stimulus_duration);
    sendSetStimulParams();
    sendGetStimulParams();
}

int ColibriStimulationSubsystem::getCurrentAmplitude() const {
    return stimulParams.stimPulseAmpl;
}

int ColibriStimulationSubsystem::getPulseDuration() const {
    return stimulParams.stimPulseDur;
}

int ColibriStimulationSubsystem::getPulseFrequency() const {
    return stimulParams.stimPulseFreq;
}

int ColibriStimulationSubsystem::getStimulationDuration() const {
    return stimulParams.stimDuration.value;
}

void ColibriStimulationSubsystem::motionAssistantStart() {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SH_START);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler) {
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME,
                   __FUNCTION__);
        return;
    }

    log->debug("[%s: %s] Sending start sh request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
}

void ColibriStimulationSubsystem::motionAssistantStop() {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SH_STOP);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler) {
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME,
                   __FUNCTION__);
        return;
    }

    log->debug("[%s: %s] Sending stop sh request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
}


bool ColibriStimulationSubsystem::getMotionAssistantIsActive() {
    return sendGetState().stepHelperIsActive;
}


void ColibriStimulationSubsystem::setMotionAssistantParams(int gyroStart, int gyroStop,
                                                           MotionAssistantLimb limb,
                                                           int stimulationPause,
                                                           int maxStimulationDuration) {

    shParams.gyroThrAngleStart = gyroStart > std::numeric_limits<unsigned char>::max()
                                 ? std::numeric_limits<unsigned char>::max()
                                 : static_cast<unsigned char>(gyroStart);
    shParams.gyroThrAngleStop = gyroStop > std::numeric_limits<unsigned char>::max()
                                ? std::numeric_limits<unsigned char>::max()
                                : static_cast<unsigned char>(gyroStop);
    shParams.limb = limb;
    shParams.minStimulationPause = stimulationPause > std::numeric_limits<unsigned char>::max()
                                   ? std::numeric_limits<unsigned char>::max()
                                   : static_cast<unsigned char>(stimulationPause);
    shParams.maxStimulDuration.value = maxStimulationDuration > std::numeric_limits<short>::max()
                                       ? std::numeric_limits<short>::max()
                                       : static_cast<short>(maxStimulationDuration);
    sendSetShParams();
    sendGetShParams();
}

int ColibriStimulationSubsystem::getGyroStart() {
    return shParams.gyroThrAngleStart;
}

int ColibriStimulationSubsystem::getGyroStop() {
    return shParams.gyroThrAngleStop;
}

MotionAssistantLimb ColibriStimulationSubsystem::getSelectedLimb() {
    return shParams.limb;
}

int ColibriStimulationSubsystem::getStimulationPause() {
    return shParams.minStimulationPause;
}

int ColibriStimulationSubsystem::getMaxStimulationDuration() {
    return shParams.maxStimulDuration.value;
}

bool ColibriStimulationSubsystem::sendGetStimulParams() {
    std::shared_ptr<ColibriCommandData> cmdData;
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler) {
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME,
                   __FUNCTION__);
        return false;
    }


    log->debug("[%s: %s] Sending get stimul params request", COLIBRI_DEVICE_CLASSNAME,
               __FUNCTION__);
    cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_STIM_PARAM);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR) {
        log->error("[%s: %s] Get stimulation params request error: 0x%02x",
                   COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, static_cast<Byte>(cmdData->getError()));
        return false;
    }

    auto responseDataLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
               responseDataLength);
    if (responseDataLength >= COLIBRI_STIM_PARAMS_DATA_LENGTH) {
        auto responseData = cmdData->getResponseData();
        log->debug("[%s: %s] Getting stimul params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

        stimulParams.stimPulseAmpl = responseData[1];
        log->debug("[%s: %s] Stimul amplitude: %d mA", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   stimulParams.stimPulseAmpl);

        stimulParams.stimPulseDur = responseData[2];
        log->debug("[%s: %s] Stimul pulse duration %d us", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   stimulParams.stimPulseDur * 10);

        stimulParams.stimPulseFreq = responseData[3];
        log->debug("[%s: %s] Stimul pulse frequency %d Hz", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   stimulParams.stimPulseFreq);

        stimulParams.stimDuration.bytes[0] = responseData[4];
        stimulParams.stimDuration.bytes[1] = responseData[5];
        log->debug("[%s: %s] Stimulation duration: %d ms", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   stimulParams.stimDuration);

        return true;
    }
    return false;
}

bool ColibriStimulationSubsystem::sendGetShParams() {
    std::shared_ptr<ColibriCommandData> cmdData;
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler) {
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME,
                   __FUNCTION__);
        return false;
    }


    log->debug("[%s: %s] Sending get sh params request", COLIBRI_DEVICE_CLASSNAME,
               __FUNCTION__);
    cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_SH_PARAM);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR) {
        log->error("[%s: %s] Get step helper params request error: 0x%02x",
                   COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, static_cast<Byte>(cmdData->getError()));
        return false;
    }

    auto responseDataLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
               responseDataLength);
    if (responseDataLength >= COLIBRI_SH_PARAMS_DATA_LENGTH) {
        auto responseData = cmdData->getResponseData();
        log->debug("[%s: %s] Getting sh params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

        shParams.analyzedAxis = responseData[0];
        log->debug("[%s: %s] Axis %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   shParams.analyzedAxis);

        shParams.gyroThrAngleStart = responseData[1];
        log->debug("[%s: %s] Gyro start: %d grad/s", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   shParams.gyroThrAngleStart);

        shParams.gyroThrAngleStop = responseData[2];
        log->debug("[%s: %s] Gyro stop %d grad/s", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   shParams.gyroThrAngleStop);

        MotionAssistantLimb limb;
        if (!parseLimbCode(responseData[3], limb)) {
            log->error("[%s: %s] Failed parse limb code %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                       responseData[5]);
            return false;
        }
        shParams.limb = limb;
        log->debug("[%s: %s] Limb code is %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__,
                   shParams.limb);

        shParams.minStimulationPause = responseData[4];

        return true;
    }
    return false;
}

bool ColibriStimulationSubsystem::sendSetStimulParams() {
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SET_STIM_PARAM);
    std::vector<unsigned char> stimParams = {1,
                                             stimulParams.stimPulseAmpl,
                                             stimulParams.stimPulseDur,
                                             stimulParams.stimPulseFreq,
                                             stimulParams.stimDuration.bytes[0],
                                             stimulParams.stimDuration.bytes[1]};
    cmdData->setRequestData(stimParams);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler){
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return false;
    }

    log->debug("[%s: %s] Sending set stimulation params request", COLIBRI_DEVICE_CLASSNAME,
               __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}

bool ColibriStimulationSubsystem::sendSetShParams() {
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::SET_SH_PARAM);
    std::vector<unsigned char> stimParams = {0, //analyzed axis must be 0
                                  shParams.gyroThrAngleStart,
                                  shParams.gyroThrAngleStop,
                                  static_cast<unsigned char>(shParams.limb),
                                  shParams.minStimulationPause};
    cmdData->setRequestData(stimParams);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler){
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return false;
    }

    log->debug("[%s: %s] Sending set sh params request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    return cmdData->getError() == ColibriCommandError::NO_ERROR;
}

ColibriStimulatorState ColibriStimulationSubsystem::sendGetState() {

    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::GET_SH_AND_STIM_STATE);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    ColibriStimulatorState state = { false, false };

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler){
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return state;
    }

    log->debug("[%s: %s] Sending get stimul and sh request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();

    if (cmdData->getError() != ColibriCommandError::NO_ERROR) return state;

    auto responseDataLength = cmdData->getResponseLength();
    log->debug("[%s: %s] Response length: %zd", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, responseDataLength);
    if (responseDataLength >= COLIBRI_STIM_STATE_PARAMS_DATA_LENGTH)
    {
        auto responseData = cmdData->getResponseData();
        log->debug("[%s: %s] Getting stimul and sh state params", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);

        state.stimulatorIsActive = responseData[0] == COLIBRI_STIMULATOR_STATE_ACTIVE;
        log->debug("[%s: %s] Stimulator state code: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, responseData[0]);

        state.stepHelperIsActive = responseData[1] == COLIBRI_SH_STATE_ACTIVE;
        log->debug("[%s: %s] Sh state code: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, responseData[1]);

        return state;
    }
    return state;
}

void ColibriStimulationSubsystem::doCalibration() {
    auto cmdData = std::make_shared<ColibriCommandData>(ColibriCommand::DO_CALIBRATION);
    auto log = LoggerFactory::getCurrentPlatformLogger();

    auto requestHandler = requestHandlerPtr.lock();
    if (!requestHandler){
        log->error("[%s: %s] Request handler been destroyed", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
        return;
    }

    log->debug("[%s: %s] Sending calibration request", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    requestHandler->sendRequest(cmdData);
    cmdData->wait();
    if (cmdData->getError() == ColibriCommandError::NO_ERROR)
        log->debug("[%s: %s] Sending calibration request succeeded", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__);
    else
        log->warn("[%s: %s] Calibration request error: %d", COLIBRI_DEVICE_CLASSNAME, __FUNCTION__, cmdData->getError());
}


*/












































