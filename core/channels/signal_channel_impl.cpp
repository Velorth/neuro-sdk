#include "channels/signal_channel_impl.h"
#include "gsl/gsl_assert"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "logger.h"

namespace Neuro {

SignalChannel::Impl::Impl(std::shared_ptr<Device> device, const ChannelInfo &info):
    mDevice(device),
    mInfo(info),
    mChannelsCount(countChannelsWithType(*device, mInfo.getType())){
    Expects(device != nullptr);
    Expects(mChannelsCount > 0);
    Expects(checkHasParameter(*device, Parameter::SamplingFrequency));
}

SignalChannel::data_container SignalChannel::Impl::readData(data_offset_t offset, data_length_t length) const {
    auto&& buffer = getDeviceBuffer();
    if (mChannelsCount <= 1){
        return buffer.readFill(offset, length, 0.0);
    }
    else {
        auto realOffset = offset * mChannelsCount;
        auto realLength = length * mChannelsCount;
        LOG_TRACE_V("Real offset: %zd, Real length: %zd, Channels count: %zd, Number: %zd", realOffset, realLength, mChannelsCount,mInfo.getIndex());
        auto allChannelsData = buffer.readFill(realOffset, realLength, 0.0);
        SignalChannel::data_container resultBuffer;
        resultBuffer.reserve(length);
        for (std::size_t i = mInfo.getIndex(); i < allChannelsData.size(); i+=mChannelsCount){
            resultBuffer.push_back(allChannelsData[i]);
        }
        Ensures(resultBuffer.size() == length);
        return resultBuffer;
    }
}

data_length_t SignalChannel::Impl::totalLength() const noexcept {
    try {
        auto&& buffer = getDeviceBuffer();
        auto totalLength = buffer.totalLength();
        if (mChannelsCount <= 1){
            return totalLength;
        }
        else {
            auto realLength = totalLength / mChannelsCount;
            auto remainder = totalLength % mChannelsCount;
            Ensures(remainder == 0);
            return realLength;
        }
    }
    catch (std::exception &){
        LOG_ERROR("Signal buffer size is not multiple of channels count");
        return 0;
    }
}

data_length_t SignalChannel::Impl::bufferSize() const noexcept {
    try {
        auto&& buffer = getDeviceBuffer();
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

std::weak_ptr<Device> SignalChannel::Impl::underlyingDevice() const noexcept {
    return mDevice;
}

sampling_frequency_t SignalChannel::Impl::samplingFrequency() const noexcept {
    try{
        auto frequency = mDevice->readParam<Parameter::SamplingFrequency>();
        return static_cast<float>(intValue(frequency));
    }
    catch (std::runtime_error &){
        return 0.f;
    }
}

const BaseBuffer<signal_sample_t> &SignalChannel::Impl::getDeviceBuffer() const noexcept {
    return mDevice->mImpl->signalBuffer();
}

}
