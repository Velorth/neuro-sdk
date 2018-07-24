#include "channels/info/channel_info.h"
#include "channels/signal_channel.h"
#include "channels/raw_signal_channel_impl.h"
#include "channels/filtered_signal_channel_impl.h"

namespace Neuro {

SignalChannel::SignalChannel(std::shared_ptr<Device> device, const ChannelInfo &channel_info) :
    BaseChannel(channel_info),
    mImpl(std::make_unique<RawSignalChannelImpl>(device, channel_info)){

}

SignalChannel::SignalChannel(std::shared_ptr<Device> device,
                             std::vector<DSP::DigitalFilterPtr<signal_sample_t>> &&filters,
                             const ChannelInfo &channel_info) :
    BaseChannel(channel_info),
    mImpl(std::make_unique<FilteredSignalChannelImpl>(device, channel_info, std::move(filters))){

}

SignalChannel::~SignalChannel(){
}

SignalChannel::length_listener_ptr
SignalChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
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
