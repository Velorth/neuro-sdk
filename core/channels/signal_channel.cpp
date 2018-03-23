#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/signal_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"

namespace Neuro {

class SignalChannel::Impl {
private:
    std::shared_ptr<Device> mDevice;

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::Signal));
        Expects(checkHasParameter(*device, Parameter::SamplingFrequency));
    }

    void setLengthChangedCallback(length_changed_callback_t callback) noexcept {

    }

    SignalChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->signalBuffer();
        return buffer.readFill(offset, length, 0.0);
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->signalBuffer();
            return buffer.totalLength();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->signalBuffer();
            return buffer.bufferSize();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    std::weak_ptr<Device> underlyingDevice() const noexcept {
        return mDevice;
    }

    sampling_frequency_t samplingFrequency() const noexcept {
        return getSamplingFrequencyValue(mDevice->readParam<Parameter::SamplingFrequency>());
    }
};

SignalChannel::SignalChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::Signal),
    mImpl(std::make_unique<Impl>(device)){

}

SignalChannel::~SignalChannel(){
}

void SignalChannel::setLengthChangedCallback(length_changed_callback_t callback) noexcept {
    mImpl->setLengthChangedCallback(callback);
}

SignalChannel::data_container SignalChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t SignalChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t SignalChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> SignalChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t SignalChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void SignalChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set dampling frequency for signal channel. It must be set for device.");
}

}
