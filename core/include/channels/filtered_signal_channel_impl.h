#ifndef FILTERED_SIGNAL_CHANNEL_IMPL_H
#define FILTERED_SIGNAL_CHANNEL_IMPL_H

#include "channels/signal_channel_impl.h"

namespace Neuro {

class FilteredSignalChannelImpl : public SignalChannel::Impl {
private:
    static constexpr const char *class_name = "FilteredSignalChannelImpl";

public:
    FilteredSignalChannelImpl(std::shared_ptr<Device> device, const ChannelInfo &info);
    SignalChannel::data_container readData(data_offset_t offset, data_length_t length) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
};

}
#endif // FILTERED_SIGNAL_CHANNEL_IMPL_H
