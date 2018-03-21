#include <algorithm>
#include "gsl/gsl_assert"
#include "device/device.h"
#include "device/device_impl.h"
#include "channels/channel_info.h"

namespace Neuro {

Device::~Device(){}

void Device::connect(){
    Expects(mImpl != nullptr);
    mImpl->connect();
}

void Device::disconnect(){
    Expects(mImpl != nullptr);
    mImpl->disconnect();
}

std::vector<ChannelInfo> Device::channels() const {
    Expects(mImpl != nullptr);
    return mImpl->channels();
}

std::vector<Command> Device::commands() const {
    Expects(mImpl != nullptr);
    return mImpl->commands();
}

std::vector<ParamPair> Device::parameters() const {
    Expects(mImpl != nullptr);
    return mImpl->parameters();
}

bool Device::execute(Command cmd) {
    Expects(mImpl != nullptr);
    return mImpl->execute(cmd);
}

void Device::setParamChangedCallback(std::function<void(Parameter)> callback) {
    mImpl->setParamChangedCallback(callback);
}

Device::Device(std::unique_ptr<DeviceImpl> impl) :
    mImpl(std::move(impl)) {
    Ensures(mImpl != nullptr);
}


bool checkHasChannel(const Device &device, const ChannelInfo &info){
    auto channels = device.channels();
    return std::find(channels.begin(), channels.end(), info) != channels.end();
}

bool checkHasCommand(const Device &device, Command cmd){
    auto commands = device.commands();
    return std::find(commands.begin(), commands.end(), cmd) != commands.end();
}

bool checkHasParameter(const Device &device, Parameter param){
    auto parameters = device.parameters();
    return std::find_if(parameters.begin(),
                     parameters.end(),
                     [param](const auto& param_pair){
                        return param_pair.first == param;
                     }) != parameters.end();
}

ParamAccess getParameterAccess(const Device &device, Parameter param){
    if (!checkHasParameter(device, param)){
        throw std::runtime_error("Device has no such parameter");
    }

    auto parameters = device.parameters();
    auto paramPair = std::find_if(parameters.begin(),
                     parameters.end(),
                     [param](const auto& param_pair){
                        return param_pair.first == param;
                     });
    return paramPair->second;
}

}
