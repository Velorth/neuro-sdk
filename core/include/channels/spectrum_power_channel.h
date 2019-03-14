#ifndef SPECTRUM_POWER_CHANNEL
#define SPECTRUM_POWER_CHANNEL

#include "channels/info/channel_info.h"
#include "channels/spectrum_channel.h"
#include <channels/ecg/ecg_channel.h>

namespace Neuro {

template <typename T>
SDK_SHARED double spectrum_power_normalized(float low_frequency, float high_frequency, const std::vector<T> &spectrum, float frequency_step) {
	auto start = static_cast<std::size_t>(low_frequency / frequency_step);
	auto stop = static_cast<std::size_t>(high_frequency / frequency_step);
	return std::accumulate(spectrum.begin() + start, spectrum.begin() + stop, 0.0) / (stop - start);
}

template <typename SpectrumDoubleChannel>
class SpectrumPowerChannel final {
public:
	using SpectrumChannelPtr = std::shared_ptr<SpectrumDoubleChannel>;
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataType = signal_sample_t;
	using DataContainer = std::vector<DataType>;

    SDK_SHARED SpectrumPowerChannel(const std::vector<SpectrumChannelPtr> &channels, 
									float low_frequency, float high_frequency, 
									std::string name = "SpectrumPower",
									double window_duration_seconds = 8.0, 
									double window_overlapping_coefficient = 0.9):
		mInfo(ChannelInfo(ChannelInfo::Type::Custom, name)),
		mSpectrumChannels(channels),
		mLengthListeners(subscribeLengthChanged(mSpectrumChannels)),
		mLowFrequency(low_frequency),
		mHighFrequency(high_frequency),
		mDurationStep(0.1),
		mWindowDuration(window_duration_seconds),
		mOverlapping(window_overlapping_coefficient){	
		if (mOverlapping > 0.99) mOverlapping = 0.99;
		else if (mOverlapping < 0.01) mOverlapping = 0.01;
		mDurationStep = mWindowDuration - mWindowDuration * mOverlapping;
    }

	SDK_SHARED ~SpectrumPowerChannel() = default;

	SDK_SHARED ChannelInfo& info() noexcept {
		return mInfo;
	}

	SDK_SHARED LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept {
		return mPowerBuffer.subscribeLengthChanged(callback);
	}

	SDK_SHARED DataContainer readData(data_offset_t offset, data_length_t length) const {
		return mPowerBuffer.readFill(offset, length, 0.0);
	}

	SDK_SHARED data_length_t totalLength() const noexcept {
		return mPowerBuffer.totalLength();
	}

	SDK_SHARED sampling_frequency_t samplingFrequency() const noexcept {
		return 1.0f /mDurationStep;
	}

	SDK_SHARED void setWindowDuration(double duration_seconds) noexcept {
		mWindowDuration = duration_seconds;
		mDurationStep = mWindowDuration - mWindowDuration * mOverlapping;
	}

	SDK_SHARED void setOverlappingCoefficient(double coefficient) noexcept {
		if (mOverlapping > 0.99) mOverlapping = 0.99;
		else if (mOverlapping < 0.01) mOverlapping = 0.01;
		mDurationStep = mWindowDuration - mWindowDuration * mOverlapping;
	}

private:
	ChannelInfo mInfo;
	SafeBuffer<signal_sample_t, 1000> mPowerBuffer;
	std::vector<SpectrumChannelPtr> mSpectrumChannels;
	std::vector<LengthListenerType> mLengthListeners;
	const float mLowFrequency;
	const float mHighFrequency;
	double mDurationStep;
	double mWindowDuration;
	double mOverlapping;
	std::unordered_map<std::size_t, double> mChannelDurationMap;
	std::mutex mChannelMapMutex;
	std::mutex mCalculationMutex;
	double mLastCalcTime{ 0.0 };

	double durationFromLength(std::size_t channel_number, std::size_t length) {
		return static_cast<double>(length) / mSpectrumChannels[channel_number]->samplingFrequency();
	}

	void setChannelDuration(const std::size_t channel_number, const double duration) {
		std::lock_guard<std::mutex> mapLock(mChannelMapMutex);
		mChannelDurationMap[channel_number] = duration;
	}

	double getMinDuration() {
		std::lock_guard<std::mutex> mapLock(mChannelMapMutex);
		return std::min_element(mChannelDurationMap.begin(), mChannelDurationMap.end())->second;
	}

	signal_sample_t getIndexOnInterval(double time,	double duration) {
		std::vector<signal_sample_t> powers(mSpectrumChannels.size());
		std::transform(mSpectrumChannels.begin(),
			mSpectrumChannels.end(),
			powers.begin(),
			[=](const SpectrumChannelPtr &channel) {
			auto offset = static_cast<std::size_t>(channel->samplingFrequency() * time);
			auto length = static_cast<std::size_t>(channel->samplingFrequency() * duration);
			auto spectrumData = channel->readData(offset, length);
			return spectrum_power_normalized(mLowFrequency, mHighFrequency, spectrumData, channel->hzPerSpectrumSample());
		});
		return std::accumulate(powers.begin(), powers.end(), 0.0) / powers.size();
	}

	void onSignalLengthChanged(const std::size_t channel_number, const std::size_t length) {
		const auto channelDuration = durationFromLength(channel_number, length);
		setChannelDuration(channel_number, channelDuration);
		const auto minDuration = getMinDuration();
		if (minDuration < mWindowDuration) {
			return;
		}

		std::unique_lock<std::mutex> calculateOnceLock(mCalculationMutex, std::defer_lock);
		if (!calculateOnceLock.try_lock())
			return;

		if (minDuration >= mLastCalcTime + mDurationStep) {
			const auto readTime = minDuration - mWindowDuration;
			mPowerBuffer.append({ getIndexOnInterval(readTime, mWindowDuration) });
			mLastCalcTime = minDuration;
		}
	}

	std::vector<LengthListenerType> subscribeLengthChanged(const std::vector<SpectrumChannelPtr> &channels) {
		std::vector<LengthListenerType> listeners;
		std::transform(channels.begin(),
			channels.end(),
			std::back_inserter(listeners),
			[=](auto &spectrum_channel) {
			return spectrum_channel->subscribeLengthChanged([this, number = mLengthListeners.size()](auto length) {
				onSignalLengthChanged(number, length);
			});
		});
		return listeners;
	}
};

}

#endif