extern "C"
{
#include "crespiration-channel.h"
}

#include "cchannel-helper.h"
#include "device/device.h"
#include "channels/device_channel.h"

using RespirationChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Respiration>;
using RespirationChannelWrap = SpecificChannelWrapper<RespirationChannelType>;
using RespirationChannelWrapPtr = std::shared_ptr<RespirationChannelWrap>;

RespirationDoubleChannel* create_RespirationDoubleChannel(Device* device_ptr, ChannelInfo info) {
	try {
		auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		const auto channelPtr = std::make_shared<RespirationChannelType>(device, channelInfo);
		return getCObjectPtr<RespirationDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int RespirationDoubleChannel_get_buffer_size(RespirationDoubleChannel* channel, size_t* out_buffer_size) {
	auto& respirationChannel = *reinterpret_cast<RespirationChannelWrapPtr *>(channel);
	return readBufferSize(*respirationChannel->channelPtr(), out_buffer_size);
}
