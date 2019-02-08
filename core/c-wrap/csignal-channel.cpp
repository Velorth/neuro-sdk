extern "C"
{
#include "csignal-channel.h"
}

#include "device/device.h"
#include "channels/device_channel.h"
#include "cchannel-helper.h"
#include "sdk_error.h"

using SignalChannelType = Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>;
using SignalChannelWrap = SpecificChannelWrapper<SignalChannelType>;
using SignalChannelWrapPtr = std::shared_ptr<SignalChannelWrap>;

SignalDoubleChannel* create_SignalDoubleChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = std::make_shared<SignalChannelType>(device);
		return getCObjectPtr<SignalDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalDoubleChannel* create_SignalDoubleChannel_info(Device *device_ptr, ChannelInfo info) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		const auto channelPtr = std::make_shared<SignalChannelType>(device, channelInfo);
		return getCObjectPtr<SignalDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalDoubleChannel* create_SignalDoubleChannel_info_filters(Device *device_ptr, ChannelInfo info, Filter *filters, size_t filter_count) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		auto digitalFilters = getCompoundFilter(filters, filter_count);
		const auto channelPtr = std::make_shared<SignalChannelType>(device, std::move(digitalFilters), channelInfo);
		return getCObjectPtr<SignalDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalDoubleChannel* create_SignalDoubleChannel_filters(Device *device_ptr, Filter *filters, size_t filter_count) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		auto digitalFilters = getCompoundFilter(filters, filter_count);
		const auto channelPtr = std::make_shared<SignalChannelType>(device, std::move(digitalFilters));
		return getCObjectPtr<SignalDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int SignalDoubleChannel_get_buffer_size(SignalDoubleChannel* channel, size_t* out_buffer_size) {
	auto& signalChannel = *reinterpret_cast<SignalChannelWrapPtr *>(channel);
	return readBufferSize(*signalChannel->channelPtr(), out_buffer_size);
}
