#include "gsl/gsl_assert"
#include "algorithm.h"
#include <algorithm>
#include "channels/spectrum_channel.h"

namespace Neuro {

class SpectrumChannel::Impl {
private:
    static constexpr std::size_t SpectrumMinAccuracy = 2;
    std::shared_ptr<BaseChannel<double>> mSourceChannel;

public:
    Impl(std::shared_ptr<BaseChannel<double>> source_channel) :
        mSourceChannel(source_channel){
        Expects(mSourceChannel != nullptr);
    }

    LengthListenerPtr subscribeLengthChanged(LengthCallbackType callback) noexcept {
        return mSourceChannel->subscribeLengthChanged(callback);
    }

    SpectrumChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto signal = mSourceChannel->readData(offset, length);
        std::size_t steps = signal.size() / spectrumLength();
        if (signal.size() % spectrumLength() != 0){
            ++steps;
            signal.resize(steps * spectrumLength());
        }

        std::vector<double> signalSpectrum(spectrumLength());
        for (std::size_t step = 0; step < steps; ++step){
            decltype(signal) signalPart(signal.begin() + spectrumLength() * step,
                                        signal.begin() + spectrumLength() * (step + 1));
            auto spectrumPart = alg::fft(signalPart);
            std::transform(signalSpectrum.begin(), signalSpectrum.end(), spectrumPart.begin(), signalSpectrum.begin(), std::plus<double>());
        }
        if (steps > 1){
            std::transform(signalSpectrum.begin(), signalSpectrum.end(), signalSpectrum.begin(), [steps](auto value){return value/steps;});
        }

        return signalSpectrum;
    }

    data_length_t totalLength() const noexcept {
        return mSourceChannel->totalLength();
    }

    sampling_frequency_t samplingFrequency() const noexcept {
        return mSourceChannel->samplingFrequency();
    }

    std::size_t spectrumLength() const noexcept{
        auto spectrumLength = static_cast<std::size_t>(mSourceChannel->samplingFrequency() * SpectrumMinAccuracy);
        return alg::next_power_2(spectrumLength);
    }

    double hzPerSpectrumSample() const noexcept {
        return mSourceChannel->samplingFrequency() / spectrumLength();
    }
};

SpectrumChannel::SpectrumChannel(std::shared_ptr<BaseChannel<double>> source_channel) :
    BaseChannel(ChannelInfo(ChannelInfo::Type::Custom,
                            std::string("Spectrum ")+source_channel->info().getName())),
    mImpl(std::make_unique<Impl>(source_channel)){

}

SpectrumChannel::~SpectrumChannel(){
}

SpectrumChannel::LengthListenerPtr
SpectrumChannel::subscribeLengthChanged(LengthCallbackType callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

SpectrumChannel::data_container
SpectrumChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t SpectrumChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

sampling_frequency_t SpectrumChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

double SpectrumChannel::hzPerSpectrumSample() const noexcept {
    return mImpl->hzPerSpectrumSample();
}

}
