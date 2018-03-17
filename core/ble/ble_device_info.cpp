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

#include "ble/ble_device_info.h"

using std::string;

namespace Neuro {

BleDeviceInfo::BleDeviceInfo()

{
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Colibri_R",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_RED)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Colibri_B",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_BLUE)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Colibri_Y",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_YELLOW)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Colibri_W",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_WHITE)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Callibri_R",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_RED)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Callibri_B",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_BLUE)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Callibri_Y",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_YELLOW)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("Neurotech_Callibri_W",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Callibri,
                                                                                  DeviceGattType::COLIBRI_WHITE)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("NeuroBLE",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Brainbit,
                                                                                  DeviceGattType::BRAINBIT)));
    nameToTypeMap.insert(
            std::pair<std::string, std::pair<DeviceType, DeviceGattType>>("BrainBit",
                                                                          std::pair<DeviceType, DeviceGattType>(
                                                                                  DeviceType::Brainbit,
                                                                                  DeviceGattType::BRAINBIT)));
}

std::unique_ptr<BleDeviceInfo> BleDeviceInfo::fromDeviceName(string deviceName)
{
    std::unique_ptr<BleDeviceInfo> deviceInfo(new BleDeviceInfo());
    deviceInfo->name = deviceName;
    deviceInfo->setTypesFromName(deviceName);
    return deviceInfo;
}

void BleDeviceInfo::setTypesFromName(std::string name)
{
    if (nameToTypeMap.find(name) == nameToTypeMap.end()) {
        type = DeviceType::Unknown;
        return;
    }

    type = nameToTypeMap[name].first;
    auto gattType = nameToTypeMap[name].second;
    gattInfo = DeviceGattInfoCreator::getGattInfo(gattType);
}

}







