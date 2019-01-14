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

SpectrumDoubleChannel* create_SpectrumChannel(DoubleChannel *double_channel) {
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
