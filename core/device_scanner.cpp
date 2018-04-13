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

#include "device_scanner.h"
#include "device_factory.h"
#include "logger.h"

using std::function;
using std::shared_ptr;
using std::string;
using std::vector;
using std::unique_lock;
using std::mutex;
using std::chrono::milliseconds;

namespace Neuro {

DeviceScanner::DeviceScanner(std::unique_ptr<BleScanner> device_scanner):scanner(std::move(device_scanner)) {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->trace("[%s: %s] Neuro connection constructor", "NeuroConnection", __FUNCTION__);

    vector<string> filter;

    {
        auto brainBitValidNames = DeviceGattInfoCreator::getGattInfo(
                DeviceGattType::BRAINBIT)->getValidBtNames();
        filter.insert(filter.end(), brainBitValidNames.begin(), brainBitValidNames.end());
    }

    {
        auto colibriRedValidNames = DeviceGattInfoCreator::getGattInfo(
                DeviceGattType::COLIBRI_RED)->getValidBtNames();
        filter.insert(filter.end(), colibriRedValidNames.begin(), colibriRedValidNames.end());
    }

    {
        auto colibriBlueValidNames = DeviceGattInfoCreator::getGattInfo(
                DeviceGattType::COLIBRI_BLUE)->getValidBtNames();
        filter.insert(filter.end(), colibriBlueValidNames.begin(), colibriBlueValidNames.end());
    }

    {
        auto colibriYellowValidNames = DeviceGattInfoCreator::getGattInfo(
                DeviceGattType::COLIBRI_YELLOW)->getValidBtNames();
        filter.insert(filter.end(), colibriYellowValidNames.begin(), colibriYellowValidNames.end());
    }

    {
        auto colibriWhiteValidNames = DeviceGattInfoCreator::getGattInfo(
                DeviceGattType::COLIBRI_WHITE)->getValidBtNames();
        filter.insert(filter.end(), colibriWhiteValidNames.begin(), colibriWhiteValidNames.end());
    }

    scanner->setFilter(filter);
    auto scannerCallback = [this, log](std::unique_ptr<BleDevice> bleDevice) {
        auto neuroDevice = onNewBleDevice(std::move(bleDevice));
        if (neuroDevice) {
            log->debug("[%s: %s] Notifying device found", "NeuroConnection", __FUNCTION__);
            if (this->deviceFoundCallback) this->deviceFoundCallback(std::move(neuroDevice));
        }
    };
    scanner->subscribeDeviceFound(scannerCallback);
}

DeviceScanner::~DeviceScanner(){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Neuro connection destructor", "NeuroConnection", __FUNCTION__);
    stopScan();
}

void DeviceScanner::startScan(int timeout){

    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Start scan", "NeuroConnection", __FUNCTION__);
    if (scanner->isScanning()) stopScan();
    scanner->startScan();
    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Notifying scan state changed: %s", "NeuroConnection", __FUNCTION__, scanner->isScanning()?"TRUE":"FALSE");
    std::thread([=]{if (this->scanStateChangedCallback) scanStateChangedCallback(scanner->isScanning());}).detach();
    if (timeout > 0){

        std::thread([=]{
            LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Timeout thread", "NeuroConnection", __FUNCTION__);
            std::unique_lock<std::mutex> stopScanLock(stopScanMutex);
            LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Waiting...", "NeuroConnection", __FUNCTION__);
            auto cvStatus = stopScanCondition.wait_for(stopScanLock, std::chrono::milliseconds(timeout));
            if (cvStatus == std::cv_status::timeout) {
                LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Scan timeout", "NeuroConnection", __FUNCTION__);
                scanner->stopScan();
                if (this->scanStateChangedCallback) scanStateChangedCallback(scanner->isScanning());
                return;
            }
            LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] No timeout. Exiting thread", "NeuroConnection", __FUNCTION__);
        }).detach();
    }
}

void DeviceScanner::stopScan(){

    LoggerFactory::getCurrentPlatformLogger()->debug("[%s: %s] Stop scan", "NeuroConnection", __FUNCTION__);
    std::unique_lock<std::mutex> stopScanLock(stopScanMutex);
    stopScanCondition.notify_all();
    scanner->stopScan();
    std::thread([=]{if (this->scanStateChangedCallback) scanStateChangedCallback(scanner->isScanning());}).detach();
}

void DeviceScanner::subscribeDeviceFound(function<void(std::unique_ptr<Device>)> deviceFound){
    deviceFoundCallback = deviceFound;
}

void DeviceScanner::subscribeScanStateChanged(function<void(bool)> scanStateChanged){
    scanStateChangedCallback = scanStateChanged;
}

std::unique_ptr<Device> DeviceScanner::findDeviceByAddress(std::string address){
    auto bleDevice = scanner->getDeviceByAddress(address);
    if (!bleDevice){
        auto log = LoggerFactory::getCurrentPlatformLogger();
        log->warn("[%s: %s] Find device by address: device is null", "NeuroConnection", __FUNCTION__);
        return std::unique_ptr<Device>();
    }
    return onNewBleDevice(std::move(bleDevice));
}

std::unique_ptr<Device> DeviceScanner::onNewBleDevice(std::unique_ptr<BleDevice> ble_device) {
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Creating NeuroDevice", "NeuroConnection", __FUNCTION__);
    switch (ble_device->getDeviceType()) {
       case DeviceType::Brainbit:
            return BrainbitDeviceFactory().create(std::move(ble_device));
        case DeviceType::Callibri:
            return CallibriDeviceFactory().create(std::move(ble_device));
        default:
            throw std::runtime_error("Unable to create device of unknown type");
    }
}

void DeviceScanner::releaseDevice(std::string name, std::string address) {
    scanner->releaseDevice(name, address);
}


}













