extern "C"
{
#include "cchannels.h"
}

#include "device/device.h"
#include "channels/signal_channel.h"
#include "channels/battery_channel.h"
#include "channels/resistance_channel.h"
#include "cchannel-helper.h"
#include "sdk_error.h"

BatteryChannel* create_BatteryChannel(Device *device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = new std::shared_ptr<Neuro::BatteryChannel>(std::make_shared<Neuro::BatteryChannel>(device));
		return reinterpret_cast<BatteryChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

void BatteryChannel_delete(BatteryChannel *channel) {
	const auto batteryChannel = reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	delete batteryChannel;
}

int BatteryChannel_get_info(BatteryChannel * channel, ChannelInfo * out_info) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return getChannelInfo(*batteryChannel, out_info);
}

int BatteryChannel_read_data(BatteryChannel *channel, size_t offset, size_t length, int *out_buffer) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return readChannelData(*batteryChannel, offset, length, out_buffer);
}

int BatteryChannel_get_buffer_size(BatteryChannel* channel, size_t* out_buffer_size) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return readBufferSize(*batteryChannel, out_buffer_size);
}

int BatteryChannel_get_sampling_frequency(BatteryChannel* channel, float* out_frequency) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return readSamplingFrequency(*batteryChannel, out_frequency);
}

int BatteryChannel_set_sampling_frequency(BatteryChannel* channel, float frequency) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return setSamplingFrequency(*batteryChannel, frequency);
}

int BatteryChannel_add_length_callback(BatteryChannel *channel, void(*callback)(BatteryChannel *, size_t), ListenerHandle* handle) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	try {
		auto listener = batteryChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
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

int BatteryChannel_get_total_length(BatteryChannel* channel, size_t* out_length) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return readTotalLength(*batteryChannel, out_length);
}

SignalChannel* create_SignalChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = new std::shared_ptr<Neuro::SignalChannel>(std::make_shared<Neuro::SignalChannel>(device));
		return reinterpret_cast<SignalChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalChannel* create_SignalChannel_info(Device *device_ptr, ChannelInfo info) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
	const auto channelPtr = new std::shared_ptr<Neuro::SignalChannel>(std::make_shared<Neuro::SignalChannel>(device, channelInfo));
	return reinterpret_cast<SignalChannel *>(channelPtr);
}

SignalChannel* create_SignalChannel_info_filters(Device *device_ptr, ChannelInfo info, Filter *filters, size_t filter_count) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
	auto digitalFilters = getFilters(filters, filter_count);
	const auto channelPtr = new std::shared_ptr<Neuro::SignalChannel>(std::make_shared<Neuro::SignalChannel>(device, std::move(digitalFilters), channelInfo));
	return reinterpret_cast<SignalChannel *>(channelPtr);
}

SignalChannel* create_SignalChannel_filters(Device *device_ptr, Filter *filters, size_t filter_count) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	auto digitalFilters = getFilters(filters, filter_count);
	const auto channelPtr = new std::shared_ptr<Neuro::SignalChannel>(std::make_shared<Neuro::SignalChannel>(device, std::move(digitalFilters)));
	return reinterpret_cast<SignalChannel *>(channelPtr);
}

void SignalChannel_delete(SignalChannel* channel) {
	const auto signalChannel = reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	delete signalChannel;
}

int SignalChannel_get_info(SignalChannel * channel, ChannelInfo * out_info) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return getChannelInfo(*signalChannel, out_info);
}

int SignalChannel_read_data(SignalChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readChannelData(*signalChannel, offset, length, out_buffer);
}

int SignalChannel_get_sampling_frequency(SignalChannel* channel, float* out_frequency) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readSamplingFrequency(*signalChannel, out_frequency);
}

int SignalChannel_set_sampling_frequency(SignalChannel* channel, float frequency) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return setSamplingFrequency(*signalChannel, frequency);
}

int SignalChannel_add_length_callback(SignalChannel* channel, void(*callback)(SignalChannel *, size_t), ListenerHandle* handle) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	try {
		auto listener = signalChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
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

int SignalChannel_get_total_length(SignalChannel* channel, size_t* out_length) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readTotalLength(*signalChannel, out_length);
}

int SignalChannel_get_buffer_size(SignalChannel* channel, size_t* out_buffer_size) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readBufferSize(*signalChannel, out_buffer_size);
}

ResistanceChannel* create_ResistanceChannel_info(Device* device_ptr, ChannelInfo info) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
	const auto channelPtr = new std::shared_ptr<Neuro::ResistanceChannel>(std::make_shared<Neuro::ResistanceChannel>(device, channelInfo));
	return reinterpret_cast<ResistanceChannel *>(channelPtr);
}

void ResistanceChannel_delete(ResistanceChannel* channel) {
	auto resistanceChannel = reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	delete resistanceChannel;
}

int ResistanceChannel_get_info(ResistanceChannel* channel, ChannelInfo* out_info) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	return getChannelInfo(*resistanceChannel, out_info);
}

int ResistanceChannel_read_data(ResistanceChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	return readChannelData(*resistanceChannel, offset, length, out_buffer);
}

int ResistanceChannel_get_sampling_frequency(ResistanceChannel* channel, float* out_frequency) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	return readSamplingFrequency(*resistanceChannel, out_frequency);
}

int ResistanceChannel_set_sampling_frequency(ResistanceChannel* channel, float frequency) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	return setSamplingFrequency(*resistanceChannel, frequency);
}

int ResistanceChannel_add_length_callback(ResistanceChannel* channel, void(*callback)(ResistanceChannel*, size_t),
	ListenerHandle* handle) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel); 
	try {
		auto listener = resistanceChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
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

int ResistanceChannel_get_total_length(ResistanceChannel* channel, size_t* out_length) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	return readTotalLength(*resistanceChannel, out_length);
}

int ResistanceChannel_get_buffer_size(ResistanceChannel* channel, size_t* out_buffer_size) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::ResistanceChannel> *>(channel);
	return readBufferSize(*resistanceChannel, out_buffer_size);
}
