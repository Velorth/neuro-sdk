extern "C"
{
#include "cspectrum_power_channel.h"
}

#include "channels/spectrum_channel.h"
#include "cchannel-helper.h"
#include "channels/spectrum_power_channel.h"


using DoubleChannelWrap = DataChannelWrapper<double>;
using DoubleChannelWrapPtr = std::shared_ptr<DoubleChannelWrap>;

using SpectrumChannelType = Neuro::SpectrumChannel<DoubleChannelWrap>;
using SpectrumChannelWrap = SpecificChannelWrapper<SpectrumChannelType>;
using SpectrumWrapPtr = std::shared_ptr<SpectrumChannelWrap>;

using SpectrumPowerChannelType = Neuro::SpectrumPowerChannel<SpectrumChannelType>;
using SpectrumPowerChannelWrap = SpecificChannelWrapper<SpectrumPowerChannelType>;
using SpectrumPowerChannelWrapPtr = std::shared_ptr<SpectrumPowerChannelWrap>;

SpectrumPowerDoubleChannel* create_SpectrumPowerDoubleChannel(
	SpectrumDoubleChannel **channels,
	size_t channels_count,
	float low_freq, float high_freq,
	char *name,
	double window_duration, double overlapping_coeff) {

	std::vector<std::shared_ptr<SpectrumChannelType>> spectrumChannels(channels_count);
	try {
		std::transform(channels, channels + channels_count, spectrumChannels.begin(), [](SpectrumDoubleChannel* channel) {return (*reinterpret_cast<SpectrumWrapPtr *>(channel))->channelPtr(); });
		const auto channelPtr = std::make_shared<SpectrumPowerChannelType>(spectrumChannels, low_freq, high_freq, name, window_duration, overlapping_coeff);
		return getCObjectPtr<SpectrumPowerDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int SpectrumPowerDoubleChannel_set_window_duration(SpectrumPowerDoubleChannel* channel, double duration) {
	auto& spectrumPowerChannel = *reinterpret_cast<SpectrumPowerChannelWrapPtr *>(channel);
	try {
		spectrumPowerChannel->channelPtr()->setWindowDuration(duration);
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

int SpectrumPowerDoubleChannel_set_overlapping_coefficient(SpectrumPowerDoubleChannel* channel, double overlap) {
	auto& spectrumPowerChannel = *reinterpret_cast<SpectrumPowerChannelWrapPtr *>(channel);
	try {
		spectrumPowerChannel->channelPtr()->setOverlappingCoefficient(overlap);
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


