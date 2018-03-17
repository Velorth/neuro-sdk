#include "ble/emulator/emulated_device.h"
#include "socket.h"
#include <cassert>
#include <iostream>

namespace Neuro {

EmulatedDevice::EmulatedDevice(std::string name, std::string address):
    deviceName(name),
    deviceAddress(address){
    deviceInfo = BleDeviceInfo::fromDeviceName(name);
    static unsigned short basePort = 41001;
    mPort = basePort++;
}

void EmulatedDevice::connect(){
    std::thread([=](){
        try{
            std::mutex waitResponseMutex;
            std::condition_variable waitCondition;
            std::unique_lock<std::mutex> waitLock(waitResponseMutex);
            Net::TcpClientSocket socket("localhost", 27001);
            socket.setDataReceivedCallback([this, &waitCondition](Net::ByteBuffer data){
                std::string response = Net::convert_to_string(data);
                if (response=="CANCEL")
                    return;
                if (response.length() != 5)
                    return;
                sendPort = std::stoi(response);
                try{
                    deviceSocket.reset(new Net::UdpSocket(mPort));
                    if (deviceSocket){
                        deviceSocket->setDataReceivedCallback([=](Net::ByteBuffer data, std::string, Net::PortNumberType){
                            onDataReceived(data);
                        });
                    }
                    waitCondition.notify_all();
                }
                catch (std::runtime_error &e){
                    std::cerr<< "Failed to create device socket: " << e.what() << std::endl;
                    assert(false);
                }
            });
            if (socket.connectRemote()){
                std::string requestStr = std::string("[") + deviceAddress + std::string("]")+ std::to_string(mPort);
                try{
                    socket.send(Net::to_byte_buffer(requestStr));
                }
                catch (std::runtime_error &e){
                    std::cerr<< "Failed to send request to emulation server: " << e.what() << std::endl;
                    assert(false);
                    return;
                }
                if (waitCondition.wait_for(waitLock, std::chrono::seconds(3)) == std::cv_status::timeout ||
                        !deviceSocket){
                    mState = BleDeviceState::DISCONNECTED;
                    std::thread([=](){onDeviceStateChanged(mState, parseBleErrorType(0));}).detach();
                }else{
                    mState = BleDeviceState::CONNECTED;
                    std::thread([=](){onDeviceStateChanged(mState, parseBleErrorType(0));}).detach();
                }
            }
        }
        catch (std::runtime_error &e){
            std::cerr<< "Failed to connect to emulation server: " << e.what() << std::endl;
            assert(false);
        }
        mState = BleDeviceState::DISCONNECTED;
        std::thread([=](){onDeviceStateChanged(mState, parseBleErrorType(0));}).detach();
    }).detach();
}

void EmulatedDevice::disconnect(){
    if (mState!=BleDeviceState::CONNECTED)
        return;
    std::thread([=](){
        try{
            std::mutex waitResponseMutex;
            std::condition_variable waitCondition;
            std::unique_lock<std::mutex> waitLock(waitResponseMutex);
            Net::TcpClientSocket socket("localhost", 27001);
            socket.setDataReceivedCallback([&waitCondition, this](Net::ByteBuffer){
                waitCondition.notify_all();
            });
            if (socket.connectRemote()){
                std::string requestStr = std::string("{") + deviceAddress + std::string("}");
                try{
                    socket.send(Net::to_byte_buffer(requestStr));
                }
                catch (std::runtime_error &e){
                    std::cerr<< "Failed to send request to emulation server: " << e.what() << std::endl;
                    assert(false);
                    return;
                }
                waitCondition.wait_for(waitLock, std::chrono::seconds(3));
            }
            deviceSocket.reset();
        }
        catch (std::runtime_error &e){
            std::cerr<< "Failed to connect to emulation server: " << e.what() << std::endl;
            assert(false);
            return;
        }
        mState = BleDeviceState::DISCONNECTED;
        std::thread([=](){onDeviceStateChanged(mState, parseBleErrorType(0));}).detach();
    }).detach();
}

void EmulatedDevice::close(){
    disconnect();
}

bool EmulatedDevice::sendCommand(const std::vector<Byte> &commandData){
    if (!deviceSocket)
        return false;

    return deviceSocket->sendTo("localhost", sendPort, commandData);
}

BleDeviceState EmulatedDevice::getState() const {
    return mState;
}

std::string EmulatedDevice::getName() const {
    return deviceName;
}

std::string EmulatedDevice::getNetAddress() const {
    return deviceAddress;
}

}
