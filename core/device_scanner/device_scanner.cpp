/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neuromd.com/
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

#include <thread>
#include "device_scanner/device_scanner.h"
#include "device/device_factory.h"
#include "ble/ble_scanner.h"
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
    auto scannerCallback = [=](std::unique_ptr<BleDevice> bleDevice) {
        auto deviceName = bleDevice->getName();
        auto deviceAddress = bleDevice->getNetAddress();
        auto neuroDevice = onNewBleDevice(std::move(bleDevice));
        if (neuroDevice) {
            if (this->deviceFoundCallback){
                try {
                    this->deviceFoundCallback(std::move(neuroDevice));
                }
                catch(std::exception &e){
                    LOG_ERROR_V("Error occured in user-defined DeviceFound callback function: %s", e.what());
                }
            }
        }
    };
    scanner->subscribeDeviceFound(scannerCallback);
}

DeviceScanner::~DeviceScanner(){
    stopScan();
}

void DeviceScanner::startScan(int timeout){
    LOG_DEBUG("Start scan");
    if (scanner->isScanning())
        stopScan();

    scanner->startScan();
    LOG_DEBUG_V("Notifying scan state changed: %s", scanner->isScanning() ? "TRUE" : "FALSE");
    std::thread([=]{
        if (this->scanStateChangedCallback)
            scanStateChangedCallback(scanner->isScanning());
    }).detach();

    if (timeout > 0){
        std::thread([=]{
            std::unique_lock<std::mutex> stopScanLock(stopScanMutex);
            auto cvStatus = stopScanCondition.wait_for(stopScanLock, std::chrono::milliseconds(timeout));
            if (cvStatus == std::cv_status::timeout) {
                LOG_DEBUG("Scan timeout");
                scanner->stopScan();
                if (this->scanStateChangedCallback)
                    scanStateChangedCallback(scanner->isScanning());
                return;
            }
        }).detach();
    }
}

void DeviceScanner::stopScan(){
    LOG_DEBUG("Stop scan");
    std::unique_lock<std::mutex> stopScanLock(stopScanMutex);
    stopScanCondition.notify_all();
    scanner->stopScan();
    std::thread([=]{
        if (this->scanStateChangedCallback)
            scanStateChangedCallback(scanner->isScanning());
    }).detach();
}

void DeviceScanner::subscribeDeviceFound(function<void(std::unique_ptr<Device, DeviceDeleter>)> deviceFound){
    deviceFoundCallback = deviceFound;
}

void DeviceScanner::subscribeScanStateChanged(function<void(bool)> scanStateChanged){
    scanStateChangedCallback = scanStateChanged;
}

std::unique_ptr<Device, DeviceDeleter> DeviceScanner::findDeviceByAddress(std::string address){
    auto bleDevice = scanner->getDeviceByAddress(address);
    if (!bleDevice){
        LOG_WARN("Find device by address: device is null");
        return std::unique_ptr<Device, DeviceDeleter>(nullptr, &libDeviceDeleter);
    }
    return onNewBleDevice(std::move(bleDevice));
}

std::unique_ptr<Device, DeviceDeleter> DeviceScanner::onNewBleDevice(std::unique_ptr<BleDevice> ble_device) {
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













