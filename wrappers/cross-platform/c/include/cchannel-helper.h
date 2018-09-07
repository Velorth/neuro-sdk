#ifndef CCHANNELS_HELPER_H
#define CCHANNELS_HELPER_H

extern "C"{
#include "sdk_error.h"
}

#include <filter/digital_filter.h>
#include <channels/base_channel.h>
#include "cchannels.h"

SDK_SHARED DSP::DigitalFilterPtr<double> createFilter(Filter filter);

SDK_SHARED std::vector<DSP::DigitalFilterPtr<double>> getFilters(Filter *filters, size_t filter_count);

template <typename T>
int readChannelData(const Neuro::BaseChannel<T> &channel, size_t offset, size_t length, T *out_buffer) {
	try {
		auto data = channel.readData(offset, length);
		if (data.size() > length) {
			set_sdk_last_error("Read data length is greater than requested");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::copy(data.begin(), data.end(), out_buffer);
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

SDK_SHARED int readBufferSize(const Neuro::CommonChannelInterface &channel, size_t* out_buffer_size);

SDK_SHARED int readTotalLength(const Neuro::CommonChannelInterface &channel, size_t* out_length);

SDK_SHARED int readSamplingFrequency(const Neuro::CommonChannelInterface &channel, float* out_frequency);

SDK_SHARED int setSamplingFrequency(Neuro::CommonChannelInterface &channel, float frequency);

SDK_SHARED int getChannelInfo(Neuro::CommonChannelInterface &channel, ChannelInfo *out_frequency);

#endif