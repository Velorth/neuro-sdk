#include "channels/emulation_channel.h"
#include <channels/ecg/ecg_channel.h>
#include "algorithm.h"
#include "loop.h"

namespace Neuro {

static std::vector<double> create_sine(EmulationSine sine_params, data_offset_t offset, data_length_t length, sampling_frequency_t sampling_frequency) {
	std::vector<double> sine(length);
	if (length == 0) return sine;

	auto sineFrequencyInSamples = sine_params.FrequencyHz / sampling_frequency;
	for (auto i = offset; i < offset + length; ++i) {
		sine[i - offset] = sine_params.AmplitudeV * std::sin(2 * alg::Pi * sampling_frequency * i + sine_params.PhaseShiftRad);
	}

	return sine;
}

struct EmulationChannel::Impl final {
	Impl(const ChannelInfo &info, const std::vector<EmulationSine> &components, sampling_frequency_t sampling_frequency, data_length_t initial_length):
		mInfo(info),
		mComponents(components),
		mSamplingFrequency(sampling_frequency),
		mCurrentLength(initial_length){}

	ChannelInfo mInfo;
	const std::vector<EmulationSine> mComponents;
	const sampling_frequency_t mSamplingFrequency;
	data_length_t mCurrentLength;
	Notifier<void, data_length_t> mLengthNotifier;
	std::unique_ptr<Loop<void()>> mNotificationTimer{nullptr};
};

EmulationChannel::EmulationChannel(const std::vector<EmulationSine> &components, sampling_frequency_t sampling_frequency, data_length_t initial_length):
	mImpl(std::make_unique<Impl>(ChannelInfo(ChannelInfo::Type::Custom, "EmulationChannel"), components, sampling_frequency, initial_length)){}

EmulationChannel::EmulationChannel(EmulationChannel&&) noexcept = default;

EmulationChannel& EmulationChannel::operator=(EmulationChannel&&) noexcept = default;

EmulationChannel::~EmulationChannel() = default;

ChannelInfo& EmulationChannel::info() const noexcept {
	return mImpl->mInfo;
}

EmulationChannel::LengthListenerType EmulationChannel::subscribeLengthChanged(LengthCallbackType callback) noexcept {
	return mImpl->mLengthNotifier.addListener(callback);
}

EmulationChannel::DataContainer EmulationChannel::readData(data_offset_t offset, data_length_t length) const {
	std::vector<double> result(length);
	if (length == 0) return result;

	for (const auto &component : mImpl->mComponents) {
		auto sine = create_sine(component, offset, length, samplingFrequency());
		std::transform(result.begin(), result.end(), sine.begin(), result.begin(), [](auto left, auto right) {return left + right; });
	}

	return result;
}

data_length_t EmulationChannel::totalLength() const noexcept {
	return mImpl->mCurrentLength;
}

sampling_frequency_t EmulationChannel::samplingFrequency() const noexcept {
	return  mImpl->mSamplingFrequency;
}

void EmulationChannel::startTimer() {
	if (mImpl->mNotificationTimer == nullptr) {
		auto delay = std::chrono::duration<double>(2.f / mImpl->mSamplingFrequency);
		mImpl->mNotificationTimer = std::make_unique<Loop<void()>>([=]() {
			mImpl->mCurrentLength += 2;
			mImpl->mLengthNotifier.notifyAll(mImpl->mCurrentLength);
		}, delay);
	}
}

void EmulationChannel::stopTimer() {
	mImpl->mNotificationTimer = nullptr;
}

void EmulationChannel::reset() {
	mImpl->mCurrentLength = 0;
}


}
