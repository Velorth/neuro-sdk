#ifndef RAW_SIGNAL_CHANNEL_H
#define RAW_SIGNAL_CHANNEL_H

#include "channels/signal_channel_impl.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class RawSignalChannelImpl : public SignalChannel::Impl {
private:
    static constexpr const char *class_name = "RawSignalChannelImpl";
    static constexpr std::size_t FilteredBufferSize = 75000; //5 minutes;
    SafeBuffer<signal_sample_t, FilteredBufferSize> mFilteredBuffer;

public:
    RawSignalChannelImpl(std::shared_ptr<Device> device, const ChannelInfo &info);
    SignalChannel::data_container readData(data_offset_t offset, data_length_t length) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
};

}

#endif // RAW_SIGNAL_CHANNEL_H
