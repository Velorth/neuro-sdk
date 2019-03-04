extern "C"
{
#include "cmems-channel.h"
}

#include "device/device.h"
#include "channels/device_channel.h"
#include "cchannel-helper.h"
#include "sdk_error.h"

using MEMSChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::MEMS>;
using MEMSChannelWrap = SpecificChannelWrapper<MEMSChannelType>;
using MEMSChannelWrapPtr = std::shared_ptr<MEMSChannelWrap>;

MEMSChannel* create_MEMSChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = std::make_shared<MEMSChannelType>(device);
		return getCObjectPtr<MEMSChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int MEMSChannel_get_buffer_size(MEMSChannel* channel, size_t* out_buffer_size) {
	auto& memsChannel = *reinterpret_cast<MEMSChannelWrapPtr *>(channel);
	return readBufferSize(*memsChannel->channelPtr(), out_buffer_size);
}

int MEMSChannel_read_data(MEMSChannel* channel, size_t offset, size_t length, MEMS* out_buffer, size_t buffer_size, size_t* samples_read) {
	auto& memsChannel = *reinterpret_cast<MEMSChannelWrapPtr *>(channel);
	try {
		auto data = memsChannel->readData(offset, length);
		if (data.size() > buffer_size) {
			set_sdk_last_error("Read data length is greater than read buffer size");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::transform(data.begin(), data.end(), out_buffer, [](const Neuro::MEMS &source) {
			MEMS mems;
			mems.accelerometer.X = source.accelerometer.X;
			mems.accelerometer.Y = source.accelerometer.Y;
			mems.accelerometer.Z = source.accelerometer.Z;
			mems.gyroscope.X = source.gyroscope.X;
			mems.gyroscope.Y = source.gyroscope.Y;
			mems.gyroscope.Z = source.gyroscope.Z;

			return mems;
		});
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
