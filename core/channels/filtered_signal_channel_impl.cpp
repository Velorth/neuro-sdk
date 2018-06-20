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

std::vector<signal_sample_t> FilteredSignalChannelImpl::filterData(const std::vector<signal_sample_t> &signal_data){
    std::vector<signal_sample_t> filteredData;
    filteredData.reserve(signal_data.size());
    for (auto &sample : signal_data) {
        signal_sample_t filteredSample = sample;
        for (auto &filter : mFilterCollection){
            filteredSample = filter->filter(filteredSample);
        }
        filteredData.push_back(filteredSample);
    }
    return filteredData;
}

void FilteredSignalChannelImpl::onBufferLengthChanged(std::size_t length){
    LOG_TRACE_V("New SignalChannel data length is %zd", length);
    if (length < mFilteredBuffer.totalLength()){
        auto diff = mFilteredBuffer.totalLength() - length;
        if (diff < samplingFrequency()){
            LOG_INFO_V("Signal channel length is less then filtered data buffer length. Difference is %zd. Skipping notification.", diff);
            return;
        }
        else{
            LOG_WARN_V("Signal channel length is less then filtered data buffer length. Difference is %zd. Reloading buffer.", diff);
            mFilteredBuffer.reset();
        }
    }
    auto newDataLength = length - mFilteredBuffer.totalLength();
    if (newDataLength > 0){
        auto signalChannelOffset = length - newDataLength;
        auto newSignal = SignalChannel::Impl::readData(signalChannelOffset, newDataLength);
        auto filteredSignal = filterData(newSignal);
        mFilteredBuffer.append(filteredSignal);
    }
}


}
