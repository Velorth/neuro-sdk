extern "C"
{
#include "cchannels.h"
}

#include "device/device.h"
#include "channels/signal_channel.h"
#include "channels/battery_channel.h"
#include "sdk_error.h"

extern std::string sdk_last_error;

int free_listener_handle(ListenerHandle *handle) {

}

template <typename T>
int readChannelData(const Neuro::BaseChannel<T> &channel, size_t offset, size_t length, T *out_buffer) {
	try {
		auto data = channel.readData(offset, length);
		if (data.size() > length) {
			sdk_last_error = "Read data length is greater than requested";
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::copy(data.begin(), data.end(), out_buffer);
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int readBufferSize(const Neuro::CommonChannelInterface &channel, size_t* out_buffer_size) {
	try {
		*out_buffer_size = channel.bufferSize();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int readSamplingFrequency(const Neuro::CommonChannelInterface &channel, float* out_frequency) {
	try {
		*out_frequency = channel.samplingFrequency();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

BatteryChannel* create_BatteryChannel(Device *device_ptr, ChannelInfo info) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
}

void BatteryChannel_delete(BatteryChannel *channel) {
	const auto batteryChannel = reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	delete batteryChannel;
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

int BatteryChannel_add_length_callback(BatteryChannel* channel, void(* callback)(size_t), ListenerHandle* handle) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
}

int BatteryChannel_get_total_length(BatteryChannel* channel, size_t* out_length) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
}

void SignalChannel_delete(SignalChannel* channel) {
	const auto batteryChannel = reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	delete batteryChannel;
}

int SignalChannel_read_data(SignalChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readChannelData(*signalChannel, offset, length, out_buffer);
}

int SignalChannel_get_sampling_frequency(SignalChannel* channel, float* out_frequency) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readSamplingFrequency(*signalChannel, out_frequency);
}

int SignalChannel_add_length_callback(SignalChannel* channel, void(* callback)(size_t), ListenerHandle* handle) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
}

int SignalChannel_get_total_length(SignalChannel* channel, size_t* out_length) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
}

int SignalChannel_get_buffer_size(SignalChannel* channel, size_t* out_buffer_size) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readBufferSize(*signalChannel, out_buffer_size);
}