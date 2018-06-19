#include "gsl/gsl_assert"
#include "channels/raw_signal_channel_impl.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "logger.h"

namespace Neuro {

RawSignalChannelImpl::RawSignalChannelImpl(std::shared_ptr<Device> device, const ChannelInfo &info) :
    SignalChannel::Impl(device, info){
    auto&& buffer = getDeviceBuffer();
    mSignalLengthListener = buffer.subscribeLengthChanged([=](std::size_t){
        mLengthNotifier.notifyAll(totalLength());
    });
}

SignalChannel::length_listener_ptr RawSignalChannelImpl::subscribeLengthChanged(SignalChannel::length_callback_t callback) noexcept {
    try{
        return mLengthNotifier.addListener(callback);
    }
    catch(...){
        return nullptr;
    }
}


}
