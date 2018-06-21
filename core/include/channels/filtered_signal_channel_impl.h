#ifndef FILTERED_SIGNAL_CHANNEL_IMPL_H
#define FILTERED_SIGNAL_CHANNEL_IMPL_H

#include "channels/signal_channel_impl.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class FilteredSignalChannelImpl : public SignalChannel::Impl {
public:
    FilteredSignalChannelImpl(std::shared_ptr<Device> device,
                              const ChannelInfo &info,
                              std::vector<DSP::DigitalFilterPtr<signal_sample_t>> &&filters);

    SignalChannel::length_listener_ptr subscribeLengthChanged(SignalChannel::length_callback_t callback) noexcept override;
    SignalChannel::data_container readData(data_offset_t offset, data_length_t length) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;

private:
    static constexpr const char *class_name = "FilteredSignalChannelImpl";
    static constexpr std::size_t FilteredBufferSize = 150000; //10 minutes for 250 Hz sampling frequency;
    SignalChannel::length_listener_ptr mSignalLengthListener;
    std::vector<DSP::DigitalFilterPtr<signal_sample_t>> mFilterCollection;
    SafeBuffer<signal_sample_t, FilteredBufferSize> mFilteredBuffer;

    std::vector<signal_sample_t> filterData(const std::vector<signal_sample_t> &signal_data);
    void onBufferLengthChanged(std::size_t length);
};

}
#endif // FILTERED_SIGNAL_CHANNEL_IMPL_H
