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

#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace Neuro {

enum class DeviceGattType
{
    BRAINBIT,
    COLIBRI_RED,
    COLIBRI_BLUE,
    COLIBRI_YELLOW,
    COLIBRI_WHITE
};

class DeviceGattInfo
{
public:
    virtual ~DeviceGattInfo(){}

    virtual std::string deviceServiceUUID() = 0;
    virtual std::string rxCharacteristicUUID() = 0;
    virtual std::string txCharacteristicUUID() = 0;
    virtual std::string statusCharacteristicUUID() = 0;
    virtual std::vector<std::string> getValidBtNames() = 0;

    std::string genericAccessUUID() {return "00001800-0000-1000-8000-00805F9B34FB";}
    std::string cccd() {return "00002902-0000-0000-0000-000000000000";}
};

class BrainbitGattInfo:public DeviceGattInfo
{
public:
    ~BrainbitGattInfo(){}

    std::string deviceServiceUUID(){return "6E400001-B534-F393-68A9-E50E24DCCA9E";}
    std::string rxCharacteristicUUID(){return "6E400004-B534-F393-68A9-E50E24DCCA9E";}
    std::string txCharacteristicUUID(){return "6E400003-B534-F393-68A9-E50E24DCCA9E";}
    std::string statusCharacteristicUUID(){return "6E400002-B534-F393-68A9-E50E24DCCA9E";}
    std::vector<std::string> getValidBtNames() { return {"NeuroBLE", "BrainBit"}; }
};

class ColibriRedGattInfo:public DeviceGattInfo
{
public:
    ~ColibriRedGattInfo(){}

    std::string deviceServiceUUID(){return "3D2F0001-D6B9-11E4-88CF-0002A5D5C51B";}
    std::string rxCharacteristicUUID(){return "3D2F0003-D6B9-11E4-88CF-0002A5D5C51B";}
    std::string txCharacteristicUUID(){return "3D2F0002-D6B9-11E4-88CF-0002A5D5C51B";}
    std::string statusCharacteristicUUID(){return "";}
    std::vector<std::string> getValidBtNames(){return {"Neurotech_Colibri_R",
                                                       "Neurotech_Callibri_R",
                                                       "Callibri_Red"};}
};

class ColibriBlueGattInfo:public DeviceGattInfo
{
public:
    ~ColibriBlueGattInfo(){}

    std::string deviceServiceUUID(){return "67CF0001-FA71-11E5-80B7-0002A5D5C51B";}
    std::string rxCharacteristicUUID(){return "67CF0003-FA71-11E5-80B7-0002A5D5C51B";}
    std::string txCharacteristicUUID(){return "67CF0002-FA71-11E5-80B7-0002A5D5C51B";}
    std::string statusCharacteristicUUID(){return "";}
    std::vector<std::string> getValidBtNames(){return {"Neurotech_Colibri_B",
                                                       "Neurotech_Callibri_B",
                                                       "Callibri_Blue"};}
};

class ColibriYellowGattInfo:public DeviceGattInfo
{
public:
    ~ColibriYellowGattInfo(){}

    std::string deviceServiceUUID(){return "77FF0001-FA66-11E5-B501-0002A5D5C51B";}
    std::string rxCharacteristicUUID(){return "77FF0003-FA66-11E5-B501-0002A5D5C51B";}
    std::string txCharacteristicUUID(){return "77FF0002-FA66-11E5-B501-0002A5D5C51B";}
    std::string statusCharacteristicUUID(){return "";}
    std::vector<std::string> getValidBtNames(){return {"Neurotech_Colibri_Y",
                                                       "Neurotech_Callibri_Y",
                                                       "Callibri_Yellow"};}
};

class ColibriWhiteGattInfo:public DeviceGattInfo
{
public:
    ~ColibriWhiteGattInfo(){}

    std::string deviceServiceUUID(){return "B9390001-FA71-11E5-A787-0002A5D5C51B";}
    std::string rxCharacteristicUUID(){return "B9390003-FA71-11E5-A787-0002A5D5C51B";}
    std::string txCharacteristicUUID(){return "B9390002-FA71-11E5-A787-0002A5D5C51B";}
    std::string statusCharacteristicUUID(){return "";}
    std::vector<std::string> getValidBtNames(){return {"Neurotech_Colibri_W",
                                                       "Neurotech_Callibri_W",
                                                       "Callibri_White"};}
};

class DeviceGattInfoCreator
{
public:
    static std::shared_ptr<DeviceGattInfo> getGattInfo(DeviceGattType gattType)
    {
        switch (gattType)
        {
            case DeviceGattType::BRAINBIT:
                return std::make_shared<BrainbitGattInfo>();
            case DeviceGattType::COLIBRI_RED:
                return std::make_shared<ColibriRedGattInfo>();
            case DeviceGattType::COLIBRI_BLUE:
                return std::make_shared<ColibriBlueGattInfo>();
            case DeviceGattType::COLIBRI_YELLOW:
                return std::make_shared<ColibriYellowGattInfo>();
            case DeviceGattType::COLIBRI_WHITE:
                return std::make_shared<ColibriWhiteGattInfo>();
            default:
                return std::shared_ptr<DeviceGattInfo>();
        }
    }
};

enum class DeviceType
{
    Brainbit,
    Callibri,
    Unknown
};

class BleDeviceInfo
{
public:
    ~BleDeviceInfo(){}

    static std::unique_ptr<BleDeviceInfo> fromDeviceName(std::string);

    std::string getName() const {return name;}
    DeviceType getDeviceType() const {return type;}
    std::shared_ptr<DeviceGattInfo> getGattInfo() const {return gattInfo;}
private:
    BleDeviceInfo();

    std::unordered_map<std::string, std::pair<DeviceType, DeviceGattType>> nameToTypeMap;
    std::string name;
    DeviceType type;
    std::shared_ptr<DeviceGattInfo> gattInfo;

    void setTypesFromName(std::string name);
};

class UnknownDeviceTypeException: public std::exception
{
public:
    UnknownDeviceTypeException(const char* what):_what(what){}
    virtual const char* what() const throw()
    {
        return _what;
    }
private:
    const char* _what;
};

}
#endif //DEVICE_INFO_H
