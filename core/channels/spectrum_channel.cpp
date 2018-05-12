#include "gsl/gsl_assert"
#include "algorithm.h"
#include "channels/channel_info.h"
#include "channels/spectrum_channel.h"

namespace Neuro {

class SpectrumChannel::Impl {
private:
    std::shared_ptr<BaseChannel<double>> mSourceChannel;

public:
    Impl(std::shared_ptr<BaseChannel<double>> source_channel) :
        mSourceChannel(source_channel){
        Expects(mSourceChannel != nullptr);
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        return mSourceChannel->subscribeLengthChanged(callback);
    }

    SpectrumChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto signal = mSourceChannel->readData(offset, length);
        signal.resize(alg::next_power_2(signal));
        auto spectrum = alg::fft(signal);
    }

    data_length_t totalLength() const noexcept {
        return mSourceChannel->totalLength();
    }

    data_length_t bufferSize() const noexcept {
        return mSourceChannel->bufferSize();
    }

    std::weak_ptr<Device> underlyingDevice() const noexcept {
        return mSourceChannel->underlyingDevice();
    }

    sampling_frequency_t samplingFrequency() const noexcept {
        return mSourceChannel->samplingFrequency();
    }
};

SpectrumChannel::SpectrumChannel(std::shared_ptr<BaseChannel<double>> source_channel) :
    BaseChannel(ChannelInfo(ChannelInfo::Type::Custom,
                            std::string("Spectrum ")+source_channel->info().getName())),
    mImpl(std::make_unique<Impl>(source_channel)){

}

SpectrumChannel::~SpectrumChannel(){
}

SpectrumChannel::length_listener_ptr
SpectrumChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

SpectrumChannel::data_container
SpectrumChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t SpectrumChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t SpectrumChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> SpectrumChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t SpectrumChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void SpectrumChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set sampling frequency for spectrum channel. It must be set for source channel.");
}

}
