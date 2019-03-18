#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <functional>
#include <memory>
#include "algorithm.h"
#include "common_types.h"
#include "info/channel_info.h"

namespace Neuro {

template <typename Channel>
class SpectrumChannel final {
public:
	using DataType = typename Channel::DataType;
	using DataContainer = std::vector<DataType>;
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;

	explicit SpectrumChannel(std::shared_ptr<Channel> source_channel) :
		mInfo(ChannelInfo(ChannelInfo::Type::Custom,
			std::string("Spectrum ") + source_channel->info().getName())),
		mSourceChannel(source_channel) {
	}

	ChannelInfo& info() noexcept {
		return mInfo;
	}

	const ChannelInfo& info() const noexcept {
		return mInfo;
	}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept {
		return mSourceChannel->subscribeLengthChanged(callback);
	}

	template <typename WindowFunction = alg::hamming_window>
	DataContainer readData(data_offset_t offset, data_length_t length, WindowFunction window_function = WindowFunction()) const {
		if (offset >= mSourceChannel->totalLength()) {
			return std::vector<double> (spectrumLength());
		}
		if (offset + length > mSourceChannel->totalLength()) {
			length = mSourceChannel->totalLength() - offset;
		}
		auto signal = mSourceChannel->readData(offset, length);
		std::size_t steps = signal.size() / spectrumLength();
		if (signal.size() % spectrumLength() != 0) {
			++steps;
			auto newSize = steps * spectrumLength();
			auto oldSize = signal.size();
			auto diff = newSize - oldSize;
			signal.resize(newSize);
			while (oldSize * 2 <= newSize) {
				std::copy_n(signal.begin(), oldSize, signal.begin() + oldSize);
				oldSize *= 2;
				diff = newSize - oldSize;
			}
			if (diff > 0) std::copy_n(signal.begin(), diff, signal.end() - diff);
		}

		std::vector<double> signalSpectrum(spectrumLength());
		for (std::size_t step = 0; step < steps; ++step) {

			decltype(signal) signalPart(spectrumLength());
			window_function(signal.begin() + spectrumLength() * step, signal.begin() + spectrumLength() * (step + 1), signalPart.begin());
			auto spectrumPart = alg::fft(signalPart);
			std::transform(signalSpectrum.begin(), signalSpectrum.end(), spectrumPart.begin(), signalSpectrum.begin(), std::plus<>());
		}
		if (steps > 1) {
			std::transform(signalSpectrum.begin(), signalSpectrum.end(), signalSpectrum.begin(), [steps](auto value) {return value / steps; });
		}

		return signalSpectrum;
	}

	data_length_t totalLength() const noexcept {
		return mSourceChannel->totalLength();
	}

	sampling_frequency_t samplingFrequency() const noexcept {
		return mSourceChannel->samplingFrequency();
	}

	std::size_t spectrumLength() const noexcept {
		auto spectrumLength = static_cast<std::size_t>(mSourceChannel->samplingFrequency() * SpectrumMinAccuracy);
		return alg::next_power_2(spectrumLength);
	}

	double hzPerSpectrumSample() const noexcept {
		return mSourceChannel->samplingFrequency() / spectrumLength();
	}

private:
	static constexpr std::size_t SpectrumMinAccuracy = 8;
	ChannelInfo mInfo;
	std::shared_ptr<Channel> mSourceChannel;
};



}
#endif // SPECTRUM_CHANNEL_H
