#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/respiration_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"

namespace Neuro {

class RespirationChannel::Impl {
private:
    static constexpr sampling_frequency_t RespirationSampligFrequency = 20.f;
    std::shared_ptr<Device> mDevice;

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::Respiration()));
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            auto&& buffer = mDevice->mImpl->respirationBuffer();
            return buffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    RespirationChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->respirationBuffer();
        return buffer.readFill(offset, length, 0.0);
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->respirationBuffer();
            return buffer.totalLength();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->respirationBuffer();
            return buffer.bufferSize();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    std::weak_ptr<Device> underlyingDevice() const noexcept {
        return mDevice;
    }

    sampling_frequency_t samplingFrequency() const noexcept {
        return RespirationSampligFrequency;
    }
};

RespirationChannel::RespirationChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::Respiration()),
    mImpl(std::make_unique<Impl>(device)){

}

RespirationChannel::~RespirationChannel(){
}

RespirationChannel::length_listener_ptr
RespirationChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

RespirationChannel::data_container RespirationChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t RespirationChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t RespirationChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> RespirationChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t RespirationChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void RespirationChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set sampling frequency for respiration channel. It is fixed");
}

}
