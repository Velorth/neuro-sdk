extern "C"
{
#include "corientation-channel.h"
}

#include "device/device.h"
#include "channels/device_channel.h"
#include "cchannel-helper.h"
#include "sdk_error.h"

using OrientationChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Orientation>;
using OrientationChannelWrap = SpecificChannelWrapper<OrientationChannelType>;
using OrientationChannelWrapPtr = std::shared_ptr<OrientationChannelWrap>;

OrientationChannel* create_OrientationChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = std::make_shared<OrientationChannelType>(device);
		return getCObjectPtr<OrientationChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int OrientationChannel_get_buffer_size(OrientationChannel* channel, size_t* out_buffer_size) {
	auto& orientationChannel = *reinterpret_cast<OrientationChannelWrapPtr *>(channel);
	return readBufferSize(*orientationChannel->channelPtr(), out_buffer_size);
}

int OrientationChannel_read_data(OrientationChannel* channel, size_t offset, size_t length, Quaternion* out_buffer, size_t buffer_size, size_t* samples_read) {
	auto& orientationChannel = *reinterpret_cast<OrientationChannelWrapPtr *>(channel);
	try {
		auto data = orientationChannel->readData(offset, length);
		if (data.size() > buffer_size) {
			set_sdk_last_error("Read data length is greater than read buffer size");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::transform(data.begin(), data.end(), out_buffer, [](const Neuro::Quaternion &source) { return Quaternion{ source.W, source.X, source.Y, source.Z }; });
		*samples_read = data.size();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}
