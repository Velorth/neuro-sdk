#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/resistance_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/base_buffer.h"
#include "event_notifier.h"
#include "logger.h"

namespace Neuro {

class ResistanceChannel::Impl {
private:
    static constexpr const char *class_name = "ResistanceChannelImpl";
    std::shared_ptr<Device> mDevice;
    const ChannelInfo mInfo;
    const std::size_t mChannelsCount;
    mutable Notifier<void, data_length_t> mLengthNotifier{class_name};
    ResistanceChannel::length_listener_ptr mLengthListener;

    const BaseBuffer<resistance_sample_t> &getDeviceBuffer() const noexcept {
        return mDevice->mImpl->signalBuffer();
    }

public:
    Impl(std::shared_ptr<Device> device, const ChannelInfo &channel_info) :
        mDevice(device),
        mInfo(channel_info),
        mChannelsCount(countChannelsWithType(*device, mInfo.getType())){
        Expects(device != nullptr);
        Expects(checkHasChannel(*device, mInfo));
        auto&& buffer = getDeviceBuffer();
        mLengthListener = buffer.subscribeLengthChanged([=](std::size_t){
            mLengthNotifier.notifyAll(totalLength());
        });
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            return mLengthNotifier.addListener(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    ResistanceChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        auto&& buffer = getDeviceBuffer();
        if (mChannelsCount <= 1){
            return buffer.readFill(offset, length, 0.0);
        }
        else {
            auto realOffset = offset * mChannelsCount;
            auto realLength = length * mChannelsCount;
            LOG_TRACE_V("Real offset: %zd, Real length: %zd, Channels count: %zd, Number: %zd", realOffset, realLength, mChannelsCount,mInfo.getIndex());
            auto allChannelsData = buffer.readFill(realOffset, realLength, 0.0);
            ResistanceChannel::data_container resultBuffer;
            resultBuffer.reserve(length);
            for (std::size_t i = mInfo.getIndex(); i < allChannelsData.size(); i+=mChannelsCount){
                resultBuffer.push_back(allChannelsData[i]);
            }
            Ensures(resultBuffer.size() == length);
            return resultBuffer;
        }
    }

    data_length_t totalLength() const noexcept {
        try {
            auto&& buffer = getDeviceBuffer();
            auto totalLength = buffer.totalLength();
            if (mChannelsCount <= 1){
                return totalLength;
            }
            else {
                auto realLength = totalLength / mChannelsCount;
                auto remainder = totalLength % mChannelsCount;
                Ensures(remainder == 0);
                return realLength;
            }
        }
        catch (std::exception &){
            LOG_ERROR("Resistance buffer size is not multiple of channels count");
            return 0;
        }
    }

    data_length_t bufferSize() const noexcept {
        try {
            auto&& buffer = getDeviceBuffer();
            auto bufferSize = buffer.bufferSize();
            auto realSize = bufferSize / mChannelsCount;
            auto remainder = bufferSize % mChannelsCount;
            Expects(remainder == 0);
            return realSize;
        }
        catch (std::exception &){
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
