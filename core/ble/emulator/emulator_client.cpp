#include "ble/emulator/emulator_client.h"
#include "ble/emulator/emulated_device.h"
#include "socket.h"
#include <thread>
#include <algorithm>
#include <iostream>
#include <cassert>

namespace Neuro {

EmulatorClient::EmulatorClient(){
    Net::init_sockets();
}

EmulatorClient::~EmulatorClient(){
    Net::sockets_cleanup();
}

void EmulatorClient::startScan(){
    if (scanning.load())
        return;

    foundAddresses.clear();
    scanning.store(true);
    std::thread([=](){
        std::mutex waitResponseMutex;
        std::condition_variable waitCondition;
        while(scanning.load()){
            try{
                std::unique_lock<std::mutex> waitLock(waitResponseMutex);
                Net::TcpClientSocket socket(std::string("localhost"), 27001);
                socket.setDataReceivedCallback([&waitCondition, this](Net::ByteBuffer data){
                    auto devListString = Net::convert_to_string(data);
                    std::string::size_type pos = 0;
                    while(pos!=std::string::npos){
                        pos = devListString.find("<", pos);
                        if (pos == std::string::npos || pos >=devListString.length()-1)
                            break;
                        pos++;
                        auto delPos = devListString.find("|", pos);
                        if (delPos == std::string::npos)
                            break;
                        auto name = devListString.substr(pos, delPos-pos);
                        if (std::find(deviceFilter.begin(), deviceFilter.end(), name) == deviceFilter.end())
                            break;
                        auto closePos = devListString.find(">", delPos+1);
                        if (closePos == std::string::npos)
                            break;
                        auto address = devListString.substr(delPos+1, closePos - delPos - 1);
                        if (std::find(foundAddresses.begin(), foundAddresses.end(), address) == foundAddresses.end()){
                            foundAddresses.push_back(address);
                            if (deviceFoundCallback) {
                                try{
                                deviceFoundCallback(std::unique_ptr<EmulatedDevice>(new EmulatedDevice(name, address)));
                                }
                                catch(std::runtime_error &e){
                                    std::cerr<< "Failed to create emualtion device: " << e.what() << std::endl;
                                    assert(false);
                                }
                            }
                        }
                    }
                    waitCondition.notify_all();
                });
                socket.connectRemote();
                waitCondition.wait_for(waitLock, std::chrono::seconds(3));
            }
            catch (std::runtime_error &e){
                std::cerr<< "Failed to connect to emulation server: " << e.what() << std::endl;
                assert(false);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}

void EmulatorClient::stopScan(){
    scanning.store(false);
}

std::unique_ptr<BleDevice> EmulatorClient::getDeviceByAddress(std::string address){
    static_cast<void>(address);
    return std::unique_ptr<BleDevice>();
}

void EmulatorClient::setFilter(std::vector<std::string> filter){
    deviceFilter = filter;
}

void EmulatorClient::subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)> callback){
    deviceFoundCallback = callback;
}

bool EmulatorClient::isScanning(){
    return scanning.load();
}

void EmulatorClient::releaseDevice(std::string name, std::string address){
    auto devIt = std::find(foundAddresses.begin(), foundAddresses.end(), address);
    if (devIt != foundAddresses.end()){
        foundAddresses.erase(devIt);
    }
}

}
