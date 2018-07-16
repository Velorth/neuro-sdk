#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/resistance_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"

namespace Neuro {

class ResistanceChannel::Impl {
private:
    std::shared_ptr<Device> mDevice;

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::Resistance()));
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            auto&& buffer = mDevice->mImpl->resistanceBuffer();
            return buffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    ResistanceChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->resistanceBuffer();
        return buffer.readFill(offset, length, 0.0);
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->resistanceBuffer();
            return buffer.totalLength();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->resistanceBuffer();
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
        try{
            auto frequency = mDevice->readParam<Parameter::SamplingFrequency>();
            return static_cast<float>(intValue(frequency));
        }
        catch (std::runtime_error &){
            return 0.f;
        }
    }
};

ResistanceChannel::ResistanceChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::Resistance()),
    mImpl(std::make_unique<Impl>(device)){

}

ResistanceChannel::~ResistanceChannel(){
}

ResistanceChannel::length_listener_ptr
ResistanceChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

ResistanceChannel::data_container ResistanceChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t ResistanceChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t ResistanceChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> ResistanceChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t ResistanceChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void ResistanceChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set dampling frequency for resistance channel.");
}

}
