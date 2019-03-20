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

int device_subscribe_signal_channel_data_received(Device *device_ptr, ChannelInfo info,	void(*callback)(Device*, ChannelInfo, SignalDataArray, void*), ListenerHandle *listener_handle, void* user_data) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		auto listener = device->subscribeDataReceived<Neuro::ChannelInfo::Type::Signal>([device_ptr, callback, info, user_data](const auto &data) {
			if (callback != nullptr) {
				SignalDataArray dataArray;
				using  TargetDataType = std::remove_pointer_t<decltype(dataArray.data_array)>;
				dataArray.samples_count = data.PacketData.size();
				const auto memorySize = dataArray.samples_count * sizeof(TargetDataType);
				dataArray.data_array = reinterpret_cast<TargetDataType *>(malloc(memorySize));
				std::copy(data.PacketData.begin(), data.PacketData.end(), dataArray.data_array);
				dataArray.first_samples_number = data.FirstSampleNumber;
				callback(device_ptr, info, dataArray, user_data);
			}
		}, Neuro::ChannelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index));
		if (listener == nullptr) {
			throw std::runtime_error("Failed to subscribe length changed event: length listenr is null");
		}
		*listener_handle = reinterpret_cast<ListenerHandle>(new decltype(listener)(listener));
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

void free_SignalDataArray(SignalDataArray data_array) {
	free(data_array.data_array);
}