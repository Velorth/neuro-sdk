#ifndef CCHANNELS_HELPER_H
#define CCHANNELS_HELPER_H

extern "C"{
#include "sdk_error.h"
}

#include <filter/digital_filter.h>
#include <channels/data_channel.h>
#include "cchannels.h"

SDK_SHARED std::unique_ptr<DSP::DigitalFilter<double>> createFilter(Filter filter);

SDK_SHARED std::unique_ptr<DSP::DigitalFilter<double>> getCompoundFilter(Filter *filters, size_t filter_count);

template <typename Channel>
int readChannelData(const Channel &channel, size_t offset, size_t length, typename Channel::DataType *out_buffer) {
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

template<typename Channel>
SDK_SHARED int readBufferSize(const Channel &channel, size_t* out_buffer_size) {
	try {
		*out_buffer_size = channel.bufferSize();
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

template <typename Channel>
int readTotalLength(const Channel &channel, size_t* out_length) {
	try {
		*out_length = channel.totalLength();
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

template <typename Channel>
int readSamplingFrequency(const Channel &channel, float* out_frequency) {
	try {
		*out_frequency = channel.samplingFrequency();
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

template <typename Channel>
int getChannelInfo(Channel &channel, ChannelInfo *out_frequency) {
	try {
		auto channelInfo = channel.info();
		ChannelInfo info;
		strcpy(info.name, channelInfo.getName().c_str());
		info.type = static_cast<ChannelType>(channelInfo.getType());
		info.index = channelInfo.getIndex();
		*out_frequency = info;
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

#endif