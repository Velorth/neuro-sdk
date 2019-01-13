extern "C"
{
#include "cbattery-channel.h"
}

#include "cchannel-helper.h"
#include "channels/device_channel.h"
#include "device/device.h"

using BatteryChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>;
using BatteryChannelWrap = SpecificChannelWrapper<BatteryChannelType>;
using BatteryChannelWrapPtr = std::shared_ptr<BatteryChannelWrap>;

BatteryIntChannel* create_BatteryIntChannel(Device *device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto batteryChannelPtr = std::make_shared<BatteryChannelType>(device);
		return getCObjectPtr<BatteryIntChannel>(batteryChannelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int BatteryIntChannel_get_buffer_size(BatteryIntChannel* channel, size_t* out_buffer_size) {
	const auto batteryChannel = reinterpret_cast<BatteryChannelWrapPtr *>(channel);
	return readBufferSize(*batteryChannel, out_buffer_size);
}