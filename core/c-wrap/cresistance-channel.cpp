extern "C"
{
#include "cresistance-channel.h"
}

#include "cchannel-helper.h"
#include "device/device.h"
#include "channels/device_channel.h"

using ResistanceChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>;
using ResistanceChannelWrap = SpecificChannelWrapper<ResistanceChannelType>;
using ResistanceChannelWrapPtr = std::shared_ptr<ResistanceChannelWrap>;

ResistanceDoubleChannel* create_ResistanceDoubleChannel_info(Device* device_ptr, ChannelInfo info) {
	try {
		auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		const auto channelPtr = std::make_shared<ResistanceChannelType>(device, channelInfo);
		return getCObjectPtr<ResistanceDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int ResistanceDoubleChannel_get_buffer_size(ResistanceDoubleChannel* channel, size_t* out_buffer_size) {
	auto& resistanceChannel = *reinterpret_cast<ResistanceChannelWrapPtr *>(channel);
	return readBufferSize(*resistanceChannel->channelPtr(), out_buffer_size);
}
