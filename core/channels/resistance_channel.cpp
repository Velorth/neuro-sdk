#include "gsl/gsl_assert"
#include "channels/info/channel_info.h"
#include "channels/info/channel_notification.h"
#include "channels/resistance_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/safe_buffer.h"
#include "event_notifier.h"
#include "logger.h"

namespace Neuro {

class ResistanceChannel::Impl {
private:
    static constexpr const char *class_name = "ResistanceChannelImpl";
    static constexpr std::size_t ResistanceBufferSize = 150000; //10 minutes for 250 Hz sampling frequency;
    SafeBuffer<signal_sample_t, ResistanceBufferSize> mResistanceBuffer;
    ListenerPtr<void, const std::vector<resistance_sample_t> &> mResistanceListener;
    std::shared_ptr<Device> mDevice;
    const ChannelInfo mInfo;
    const std::size_t mChannelsCount;

    resistance_sample_t getLastResistanceValue(){
        if (mResistanceBuffer.totalLength()>0){
            return mResistanceBuffer.readFill(mResistanceBuffer.totalLength()-1, 1, 0)[0];
        }
        return 0;
    }

public:
    Impl(std::shared_ptr<Device> device, const ChannelInfo &channel_info) :
        mDevice(device),
        mInfo(channel_info),
        mChannelsCount(countChannelsWithType(*device, mInfo.getType())){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, mInfo));
        mResistanceListener = device->subscribeDataReceived<ChannelInfo::Type::Resistance>([=](const std::vector<resistance_sample_t> &data){
            if (data.size() % mChannelsCount != 0){
                LOG_ERROR_V("Wrong data length. Skipping packet");
                return;
            }
            mResistanceBuffer.append(data);
        }, mInfo);
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            return mResistanceBuffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    ResistanceChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        return mResistanceBuffer.readFill(offset, length, signal_sample_t{0});
    }

    data_length_t totalLength() const noexcept {
        return mResistanceBuffer.totalLength();
    }

    data_length_t bufferSize() const noexcept {
        return mResistanceBuffer.bufferSize();
    }

    std::weak_ptr<Device> underlyingDevice() const noexcept {
        return mDevice;
    }

    sampling_frequency_t samplingFrequency() const noexcept {
        try{
            auto frequency = mDevice->readParam<Parameter::SamplingFrequency>();
            return static_cast<float>(intValue(frequency))/mChannelsCount;
        }
        catch (std::runtime_error &){
            return 0.f;
        }
    }
};

ResistanceChannel::ResistanceChannel(std::shared_ptr<Device> device, const ChannelInfo &channel_info) :
    BaseChannel(channel_info),
    mImpl(std::make_unique<Impl>(device, channel_info)){
    Expects(channel_info.getType() == ChannelInfo::Type::Resistance);
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
