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
    const ChannelInfo mInfo;
    const std::size_t mChannelsCount;

public:
    Impl(std::shared_ptr<Device> device, const ChannelInfo &info) :
        mDevice(device),
        mInfo(info),
        mChannelsCount(countChannelsWithType(*device, mInfo.getType())){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, info));
        Expects(checkHasParameter(*device, Parameter::SamplingFrequency));
    }

    void setLengthChangedCallback(length_changed_callback_t callback) noexcept {

    }

    SignalChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->signalBuffer();
        if (mChannelsCount == 0){
            return buffer.readFill(offset, length, 0.0);
        }
        else {
            auto realOffset = offset * mChannelsCount + mInfo.mIndex;
            auto realLength = length * mChannelsCount;
            return buffer.readFill(realOffset, realLength, 0.0);
        }
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->signalBuffer();
            auto totalLength = buffer.totalLength();
            auto realLength = totalLength / mChannelsCount;
            auto remainder = totalLength % mChannelsCount;
            Expects(remainder == 0);
            return realLength;
        }
        catch (std::exception &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->signalBuffer();
            auto bufferSize = buffer.bufferSize();
            auto realSize = bufferSize / mChannelsCount;
            auto remainder = bufferSize % mChannelsCount;
            Expects(remainder == 0);
            return realSize;
        }
        catch (std::exception &){
            return 0;
        }
    }

    std::weak_ptr<Device> underlyingDevice() const noexcept {
        return mDevice;
    }

    sampling_frequency_t samplingFrequency() const noexcept {
        try{
            auto frequency = mDevice->readParam<Parameter::SamplingFrequency>();
            return static_cast<float>(intValue(frequency));
        }
        catch (std::runtime_error &){
            return 0.f;
        }
    }
};

SignalChannel::SignalChannel(std::shared_ptr<Device> device, const ChannelInfo &channel_info) :
    BaseChannel(channel_info),
    mImpl(std::make_unique<Impl>(device, channel_info)){

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
