#include "gsl/gsl_assert"
#include "algorithm.h"
#include <algorithm>
#include "channels/spectrum_channel.h"

namespace Neuro {

static constexpr std::size_t SPECTRUM_MIN_ACCURACY = 2;

SpectrumChannel::SpectrumChannel(std::shared_ptr<DataChannel<double>> source_channel) :
    DataChannel(ChannelInfo(ChannelInfo::Type::Custom,
                            std::string("Spectrum ")+source_channel->info().getName())),
	mSourceChannel(source_channel) {
	Expects(mSourceChannel != nullptr);

}

SpectrumChannel::LengthListenerPtr
SpectrumChannel::subscribeLengthChanged(LengthCallbackType callback) noexcept {
	return mSourceChannel->subscribeLengthChanged(callback);
}

SpectrumChannel::DataContainer
SpectrumChannel::readData(data_offset_t offset, data_length_t length) const {
	auto signal = mSourceChannel->readData(offset, length);
	std::size_t steps = signal.size() / spectrumLength();
	if (signal.size() % spectrumLength() != 0) {
		++steps;
		signal.resize(steps * spectrumLength());
	}

	std::vector<double> signalSpectrum(spectrumLength());
	for (std::size_t step = 0; step < steps; ++step) {
		decltype(signal) signalPart(signal.begin() + spectrumLength() * step,
			signal.begin() + spectrumLength() * (step + 1));
		auto spectrumPart = alg::fft(signalPart);
		std::transform(signalSpectrum.begin(), signalSpectrum.end(), spectrumPart.begin(), signalSpectrum.begin(), std::plus<double>());
	}
	if (steps > 1) {
		std::transform(signalSpectrum.begin(), signalSpectrum.end(), signalSpectrum.begin(), [steps](auto value) {return value / steps; });
	}

	return signalSpectrum;
}

data_length_t SpectrumChannel::totalLength() const noexcept {
	return mSourceChannel->totalLength();
}

sampling_frequency_t SpectrumChannel::samplingFrequency() const noexcept {
	return mSourceChannel->samplingFrequency();
}

std::size_t SpectrumChannel::spectrumLength() const noexcept {
	auto spectrumLength = static_cast<std::size_t>(mSourceChannel->samplingFrequency() * SPECTRUM_MIN_ACCURACY);
	return alg::next_power_2(spectrumLength);
}

double SpectrumChannel::hzPerSpectrumSample() const noexcept {
	return mSourceChannel->samplingFrequency() / spectrumLength();
}

}
