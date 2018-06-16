#include "channels/signal_channel_impl.h"
#include "gsl/gsl_assert"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"

namespace Neuro {

SignalChannel::Impl::Impl(std::shared_ptr<Device> device, const ChannelInfo &info):
    mDevice(device),
    mInfo(info),
    mChannelsCount(countChannelsWithType(*device, mInfo.getType())){
    Expects(device != nullptr);
    Expects(mChannelsCount > 0);
    Expects(checkHasParameter(*device, Parameter::SamplingFrequency));
}

SignalChannel::length_listener_ptr SignalChannel::Impl::subscribeLengthChanged(length_callback_t callback) noexcept {
    try{
        return mLengthNotifier.addListener(callback);
    }
    catch(...){
        return nullptr;
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
