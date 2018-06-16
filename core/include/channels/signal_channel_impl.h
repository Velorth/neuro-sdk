#ifndef SIGNAL_CHANNEL_IMPL_H
#define SIGNAL_CHANNEL_IMPL_H

#include "channels/signal_channel.h"
#include "signal/base_buffer.h"
#include "event_notifier.h"

namespace Neuro {

class SignalChannel::Impl {
public:
    Impl(std::shared_ptr<Device> device, const ChannelInfo &info);
    virtual ~Impl() = default;

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept;
    virtual SignalChannel::data_container readData(data_offset_t offset, data_length_t length) const = 0;
    virtual data_length_t totalLength() const noexcept = 0;
    virtual data_length_t bufferSize() const noexcept = 0;
    std::weak_ptr<Device> underlyingDevice() const noexcept;
    sampling_frequency_t samplingFrequency() const noexcept;
protected:
    std::shared_ptr<Device> mDevice;
    const ChannelInfo mInfo;
    const std::size_t mChannelsCount;
    mutable Notifier<void, data_length_t> mLengthNotifier{class_name};
    SignalChannel::length_listener_ptr mSignalLengthListener;

    const BaseBuffer<signal_sample_t> & getDeviceBuffer() const noexcept;

private:
    static constexpr const char *class_name = "SignalChannelImpl";
};

}

#endif // SIGNAL_CHANNEL_IMPL_H
