#include <algorithm>
#include <vector>
#include "device/device.h"
#include "device/device_impl.h"
#include "ble/ble_device_wrapper.h"
#include "device/callibri/callibri_impl.h"
#include "device/callibri/callibri_common_parameters.h"
#include "device/brainbit/brainbit_impl.h"

namespace Neuro {

static std::unique_ptr<DeviceImpl> device_impl_from_info(const DeviceInfo &device_info) {
	auto bleDevice = std::make_shared<BleDeviceWrapper>(device_info);
	if (bleDevice->getGattInfo()->getDeviceType() == DeviceType::Brainbit) {
		return std::make_unique<BrainbitImpl>(bleDevice);
	}
	else if (bleDevice->getGattInfo()->getDeviceType() == DeviceType::Callibri) {
		auto requestHandler = std::make_shared<CallibriRequestScheduler>();
		auto commonParams = std::make_shared<CallibriCommonParameters>(requestHandler);
		return std::make_unique<CallibriImpl>(bleDevice, requestHandler, commonParams);
	}
	else {
		throw std::runtime_error("Unknown device type");
	}
}

Device::Device(const DeviceInfo &info):
	mImpl(device_impl_from_info(info)){}

Device::Device(Device &&rhs) noexcept : mImpl(std::move(rhs.mImpl)){}

Device& Device::operator=(Device &&rhs) noexcept {
	this->swap(rhs);
	return *this;
}

Device::~Device() = default;

void Device::swap(Device &rhs) noexcept {
	using std::swap;
	swap(this->mImpl, rhs.mImpl);
}

void Device::connect(){
    mImpl->connect();
}

void Device::disconnect(){
    mImpl->disconnect();
}

std::vector<ChannelInfo> Device::channels() const {
    return mImpl->channels();
}

std::vector<Command> Device::commands() const {
    return mImpl->commands();
}

std::vector<ParamPair> Device::parameters() const {
    return mImpl->parameters();
}

bool Device::execute(Command cmd) {
    return mImpl->execute(cmd);
}

ListenerPtr<void, Parameter>  Device::setParamChangedCallback(std::function<void(Parameter)> callback) {
    return mImpl->setParamChangedCallback(callback);
}

Device::Device(std::unique_ptr<DeviceImpl> impl) :
    mImpl(std::move(impl)) {
}

void libDeviceDeleter(Device *device_ptr){
    delete device_ptr;
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

std::size_t countChannelsWithType(const Device &device, ChannelInfo::Type type){
    auto channels = device.channels();
    return std::count_if(channels.begin(),
                        channels.end(),
                        [type](const auto& channel){
                            return channel.getType() == type;
                        });
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


std::vector<std::string> DeviceTraits<Device>::validServiceUUIDs() {
	return {
		"6E400001-B534-F393-68A9-E50E24DCCA9E",
		"3D2F0001-D6B9-11E4-88CF-0002A5D5C51B",
		"67CF0001-FA71-11E5-80B7-0002A5D5C51B",
		"77FF0001-FA66-11E5-B501-0002A5D5C51B",
		"B9390001-FA71-11E5-A787-0002A5D5C51B"
	};
}

std::vector<std::string> DeviceTraits<Device>::validNames() {
	return {
		"BrainBit",
		"Brainbit",
		"Neurotech_Colibri_R",
		"Neurotech_Callibri_R",
		"Callibri_Red",
		"Neurotech_Colibri_B",
		"Neurotech_Callibri_B",
		"Callibri_Blue",
		"Neurotech_Colibri_Y",
		"Neurotech_Callibri_Y",
		"Callibri_Yellow",
		"Neurotech_Colibri_W",
		"Neurotech_Callibri_W",
		"Callibri_White"
	};
}

}
