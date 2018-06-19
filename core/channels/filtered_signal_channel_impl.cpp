#include "channels/filtered_signal_channel_impl.h"


namespace Neuro {

FilteredSignalChannelImpl::FilteredSignalChannelImpl(std::shared_ptr<Device> device,
                                                     const ChannelInfo &info,
                                                     std::vector<DSP::DigitalFilterPtr<signal_sample_t>> &&filters) :
    SignalChannel::Impl(device, info),
    mFilterCollection(std::move(filters)){
    auto&& buffer = getDeviceBuffer();
    mSignalLengthListener = buffer.subscribeLengthChanged([=](std::size_t length){
        onBufferLengthChanged(length);
    });
}

SignalChannel::length_listener_ptr FilteredSignalChannelImpl::subscribeLengthChanged(SignalChannel::length_callback_t callback) noexcept {
    return mFilteredBuffer.subscribeLengthChanged(callback);
}

SignalChannel::data_container FilteredSignalChannelImpl::readData(data_offset_t offset, data_length_t length) const {
    return mFilteredBuffer.readFill(offset, length, 0.0);
}

data_length_t FilteredSignalChannelImpl::totalLength() const noexcept {
    return mFilteredBuffer.totalLength();
}

data_length_t FilteredSignalChannelImpl::bufferSize() const noexcept {
    return mFilteredBuffer.bufferSize();
}

void FilteredSignalChannelImpl::onBufferLengthChanged(std::size_t length){

}


}
