extern "C"
{
#include "celectrode-state-channel.h"
}

#include "device/device.h"
#include "channels/device_channel.h"
#include "cchannel-helper.h"
#include "sdk_error.h"

using ElectrodeStateChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::ElectrodesState>;
using ElectrodeStateChannelWrap = SpecificChannelWrapper<ElectrodeStateChannelType>;
using ElectrodeStateChannelWrapPtr = std::shared_ptr<ElectrodeStateChannelWrap>;

ElectrodeStateChannel* create_ElectrodeStateChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = std::make_shared<ElectrodeStateChannelType>(device);
		return getCObjectPtr<ElectrodeStateChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int ElectrodeStateChannel_get_buffer_size(ElectrodeStateChannel* channel, size_t* out_buffer_size) {
	auto& orientationChannel = *reinterpret_cast<ElectrodeStateChannelWrapPtr *>(channel);
	return readBufferSize(*orientationChannel->channelPtr(), out_buffer_size);
}

int ElectrodeStateChannel_read_data(ElectrodeStateChannel* channel, size_t offset, size_t length, ElectrodeState* out_buffer, size_t buffer_size, size_t* samples_read) {
	auto& electrodesChannel = *reinterpret_cast<ElectrodeStateChannelWrapPtr *>(channel);
	try {
		auto data = electrodesChannel->readData(offset, length);
		if (data.size() > buffer_size) {
			set_sdk_last_error("Read data length is greater than read buffer size");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::transform(data.begin(), data.end(), out_buffer, [](const Neuro::ElectrodeState &state) { return static_cast<ElectrodeState>(state); });
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
