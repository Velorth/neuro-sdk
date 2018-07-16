#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/mems_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"

namespace Neuro {

class MemsChannel::Impl {
private:
    static constexpr sampling_frequency_t MemsSampligFrequency = 100.f;

    std::shared_ptr<Device> mDevice;

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::MEMS()));
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            auto&& buffer = mDevice->mImpl->memsBuffer();
            return buffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    MemsChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = mDevice->mImpl->memsBuffer();
        return buffer.readFill(offset, length, MEMS{});
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->memsBuffer();
            return buffer.totalLength();
        }
        catch (std::runtime_error &){
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = mDevice->mImpl->memsBuffer();
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
        return MemsSampligFrequency;
    }
};

MemsChannel::MemsChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::MEMS()),
    mImpl(std::make_unique<Impl>(device)){

}

MemsChannel::~MemsChannel(){
}

MemsChannel::length_listener_ptr
MemsChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

MemsChannel::data_container MemsChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t MemsChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t MemsChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> MemsChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t MemsChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void MemsChannel::setSamplingFrequency(sampling_frequency_t) {
    throw std::runtime_error("Unable set sampling frequency for MEMS channel");
}

}
