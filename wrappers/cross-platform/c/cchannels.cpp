extern "C"
{
#include "cchannels.h"
}

#include "device/device.h"
#include "filter/iir_filter.h"
#include "filter/low_pass_filter.h"
#include "filter/high_pass_filter.h"
#include "filter/band_stop_filter.h"
#include "channels/signal_channel.h"
#include "channels/battery_channel.h"
#include "sdk_error.h"

extern std::string sdk_last_error;

void free_listener_handle(ListenerHandle *handle) {
	auto handlePtr = reinterpret_cast<Neuro::ListenerPtr<void(size_t)> *>(handle);
	delete handlePtr;
}

inline DSP::DigitalFilterPtr<double> createFilter(Filter filter) {
	if (filter == LowPass_1Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<1, 2, 125>>>();
	}
	if (filter == LowPass_1Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<1, 2, 125>>>();
	}
	if (filter == LowPass_5Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<5, 3, 125>>>();
	}
	if (filter == LowPass_5Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<5, 3, 125>>>();
	}
	if (filter == LowPass_15Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<15, 4, 125>>>();
	}
	if (filter == LowPass_15Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<15, 4, 125>>>();
	}
	if (filter == LowPass_27Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<27, 4, 125>>>();
	}
	if (filter == LowPass_27Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<27, 4, 125>>>();
	}
	if (filter == LowPass_30Hz_SF250) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<30, 2, 250>>>();
	}
	if (filter == LowPass_30Hz_SF250_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<30, 2, 250>>>();
	}
	if (filter == HighPass_2Hz_SF250) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<2, 2, 250>>>();
	}
	if (filter == HighPass_2Hz_SF250_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<2, 2, 250>>>();
	}
	if (filter == HighPass_3Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<3, 2, 125>>>();
	}
	if (filter == HighPass_3Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<3, 2, 125>>>();
	}
	if (filter == HighPass_5Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<5, 3, 125>>>();
	}
	if (filter == HighPass_5Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<5, 3, 125>>>();
	}
	if (filter == HighPass_11Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<11, 3, 125>>>();
	}
	if (filter == HighPass_11Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<11, 3, 125>>>();
	}
	if (filter == BandStop_45_55Hz_SF250) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::BandStop<45, 55, 4, 250>>>();
	}
	throw std::runtime_error("Filter is not supported");
}

std::vector<DSP::DigitalFilterPtr<double>> getFilters(Filter *filters, size_t filter_count) {
	std::vector<DSP::DigitalFilterPtr<double>> digitalFilters;
	digitalFilters.reserve(filter_count);
	for (size_t i = 0; i != filter_count; ++i) {
		try {
			digitalFilters.push_back(createFilter(filters[i]));
		}
		catch (...) {
			continue;
		}
	}
	return digitalFilters;
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

int readTotalLength(const Neuro::CommonChannelInterface &channel, size_t* out_length) {
	try {
		*out_length = channel.totalLength();
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

BatteryChannel* create_BatteryChannel(Device *device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	const auto channelPtr = new std::shared_ptr<Neuro::BatteryChannel>(std::make_shared<Neuro::BatteryChannel>(device));
	return reinterpret_cast<BatteryChannel *>(channelPtr);
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

int BatteryChannel_add_length_callback(BatteryChannel *channel, void(*callback)(BatteryChannel *, size_t), ListenerHandle* handle) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	try {
		auto listener = batteryChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			sdk_last_error = "Failed to subscribe length changed event: length listenr is null";
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
		return SDK_NO_ERROR;
	}
	catch(std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch(...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int BatteryChannel_get_total_length(BatteryChannel* channel, size_t* out_length) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::BatteryChannel> *>(channel);
	return readTotalLength(*batteryChannel, out_length);
}

SignalChannel* create_SignalChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	const auto channelPtr = new std::shared_ptr<Neuro::SignalChannel>(std::make_shared<Neuro::SignalChannel>(device));
	return reinterpret_cast<SignalChannel *>(channelPtr);
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

int SignalChannel_read_data(SignalChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readChannelData(*signalChannel, offset, length, out_buffer);
}

int SignalChannel_get_sampling_frequency(SignalChannel* channel, float* out_frequency) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readSamplingFrequency(*signalChannel, out_frequency);
}

int SignalChannel_add_length_callback(SignalChannel* channel, void(*callback)(SignalChannel *, size_t), ListenerHandle* handle) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	try {
		auto listener = signalChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			sdk_last_error = "Failed to subscribe length changed event: length listenr is null";
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
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

int SignalChannel_get_total_length(SignalChannel* channel, size_t* out_length) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readTotalLength(*signalChannel, out_length);
}

int SignalChannel_get_buffer_size(SignalChannel* channel, size_t* out_buffer_size) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::SignalChannel> *>(channel);
	return readBufferSize(*signalChannel, out_buffer_size);
}