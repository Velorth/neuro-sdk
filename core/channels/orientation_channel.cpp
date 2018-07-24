#include "gsl/gsl_assert"
#include "channels/info/channel_info.h"
#include "channels/orientation_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"

namespace Neuro {

class OrientationChannel::Impl {
private:
    static constexpr sampling_frequency_t AngleSampligFrequency = 100.f;

    std::shared_ptr<Device> mDevice;

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::Orientation()));
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            auto&& buffer = mDevice->mImpl->orientationBuffer();
            return buffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    OrientationChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->orientationBuffer();
        return buffer.readFill(offset, length, Quaternion{});
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->orientationBuffer();
            return buffer.totalLength();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->orientationBuffer();
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
        return AngleSampligFrequency;
    }
};

OrientationChannel::OrientationChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::Orientation()),
    mImpl(std::make_unique<Impl>(device)){

}

OrientationChannel::~OrientationChannel(){
}

OrientationChannel::length_listener_ptr
OrientationChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

OrientationChannel::data_container OrientationChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t OrientationChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t OrientationChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> OrientationChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t OrientationChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void OrientationChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set sampling frequency for angle channel");
}

}
