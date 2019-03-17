extern "C"
{
#include "cspectrum-channel.h"
}

#include "channels/spectrum_channel.h"
#include "cchannel-helper.h"

using DoubleChannelWrap = DataChannelWrapper<double>;
using DoubleChannelWrapPtr = std::shared_ptr<DoubleChannelWrap>;

using SpectrumChannelType = Neuro::SpectrumChannel<DoubleChannelWrap>;
using SpectrumChannelWrap = SpecificChannelWrapper<SpectrumChannelType>;
using SpectrumWrapPtr = std::shared_ptr<SpectrumChannelWrap>;

SpectrumDoubleChannel* create_SpectrumDoubleChannel(DoubleChannel *double_channel) {
	auto& channel = *reinterpret_cast<DoubleChannelWrapPtr *>(double_channel);
	try {
		const auto channelPtr = std::make_shared<SpectrumChannelType>(channel);
		return getCObjectPtr<SpectrumDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int SpectrumDoubleChannel_read_data(SpectrumDoubleChannel* channel, size_t offset, size_t length, double* out_buffer,
	size_t buffer_size, size_t* samples_read, SpectrumWindow window_type) {
	auto& spectrumChannel = *reinterpret_cast<SpectrumWrapPtr *>(channel);
	try {
		std::function<std::vector<double>(size_t, size_t)> read_function;
		switch (window_type) {

			case SpectrumWindowRectangular:
				read_function = [&spectrumChannel](size_t offset, size_t length) {return spectrumChannel->channelPtr()->readData<alg::rectangle_window>(offset, length); };
				break;
			case SpectrumWindowSine:
				read_function = [&spectrumChannel](size_t offset, size_t length) {return spectrumChannel->channelPtr()->readData<alg::sine_window>(offset, length); };
				break;
			case SpectrumWindowHamming:
				read_function = [&spectrumChannel](size_t offset, size_t length) {return spectrumChannel->channelPtr()->readData<alg::hamming_window>(offset, length); };
				break;
			case SpectrumWindowBlackman:
				read_function = [&spectrumChannel](size_t offset, size_t length) {return spectrumChannel->channelPtr()->readData<alg::blackman_window>(offset, length); };
				break;
			default: throw std::runtime_error("Unknown window type");
		}
		auto data = read_function(offset, length);
		if (data.size() > buffer_size) {
			set_sdk_last_error("Read data length is greater than read buffer size");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::copy(data.begin(), data.end(), out_buffer);
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

int SpectrumDoubleChannel_get_hz_per_spectrum_sample(SpectrumDoubleChannel* channel, double* out_step) {
	auto& spectrumChannel = *reinterpret_cast<SpectrumWrapPtr *>(channel);
	try {
		*out_step = spectrumChannel->channelPtr()->hzPerSpectrumSample();
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

int SpectrumDoubleChannel_get_spectrum_length(SpectrumDoubleChannel* channel, size_t* out_spectrum_length) {
	auto& spectrumChannel = *reinterpret_cast<SpectrumWrapPtr *>(channel);
	try {
		*out_spectrum_length = spectrumChannel->channelPtr()->spectrumLength();
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
