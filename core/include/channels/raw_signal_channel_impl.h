#ifndef RAW_SIGNAL_CHANNEL_H
#define RAW_SIGNAL_CHANNEL_H

#include "channels/signal_channel_impl.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class RawSignalChannelImpl : public SignalChannel::Impl {
private:
    static constexpr const char *class_name = "RawSignalChannelImpl";
    mutable Notifier<void, data_length_t> mLengthNotifier{class_name};
    SignalChannel::length_listener_ptr mSignalLengthListener;

public:
    RawSignalChannelImpl(std::shared_ptr<Device> device, const ChannelInfo &info);
    SignalChannel::length_listener_ptr subscribeLengthChanged(SignalChannel::length_callback_t callback) noexcept override;
};

}

#endif // RAW_SIGNAL_CHANNEL_H
