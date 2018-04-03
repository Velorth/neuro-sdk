#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/angle_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"

namespace Neuro {

class AngleChannel::Impl {
private:
    static constexpr sampling_frequency_t AngleSampligFrequency = 100.f;

    std::shared_ptr<Device> mDevice;

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::Angle));
    }

    void setLengthChangedCallback(length_changed_callback_t callback) noexcept {

    }

    AngleChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->angleBuffer();
        return buffer.readFill(offset, length, Quaternion{});
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->angleBuffer();
            return buffer.totalLength();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->angleBuffer();
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

AngleChannel::AngleChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::Signal),
    mImpl(std::make_unique<Impl>(device)){

}

AngleChannel::~AngleChannel(){
}

void AngleChannel::setLengthChangedCallback(length_changed_callback_t callback) noexcept {
    mImpl->setLengthChangedCallback(callback);
}

AngleChannel::data_container AngleChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t AngleChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t AngleChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> AngleChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t AngleChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void AngleChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set sampling frequency for angle channel");
}

}
