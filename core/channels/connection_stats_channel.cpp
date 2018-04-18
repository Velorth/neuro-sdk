#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/connection_stats_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "signal/safe_buffer.h"
#include "loop.h"

namespace Neuro {

class ConnectionStatsChannel::Impl {
private:
    static constexpr const char *class_name = "ConnectionStatsChannel";
    static constexpr std::size_t BufferSize = 600; //10 minutes for 1 Hz sampling frequency
    static constexpr sampling_frequency_t DefaultFrequency = 1.f;
    std::shared_ptr<Device> mDevice;
    SafeBuffer<int, BufferSize> mBuffer;
    sampling_frequency_t mSamplingFrequency{DefaultFrequency};
    Loop<void(Impl*)> mLoop;

    void readConnectionStatsFunc(){
        auto packetsLost = static_cast<double>(mDevice->mImpl->packetsLost());
        auto packetsReceived = static_cast<double>(mDevice->mImpl->packetsReceived());
        auto percents = packetsReceived / (packetsLost + packetsReceived) * 100.0;

        mBuffer.append({static_cast<int>(percents)});
    }

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device),
        mLoop(&Impl::readConnectionStatsFunc, std::chrono::duration<sampling_frequency_t>(1.0/mSamplingFrequency), this){
        Expects(mDevice != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::ConnectionStats));
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            return mBuffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    ConnectionStatsChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        return mBuffer.readFill(offset, length, 0);
    }

    data_length_t totalLength() const noexcept {
        return mBuffer.totalLength();
    }

    data_length_t bufferSize() const noexcept {
        return mBuffer.bufferSize();
    }

    std::weak_ptr<Device> underlyingDevice() const noexcept {
        return mDevice;
    }

    sampling_frequency_t samplingFrequency() const {
        return mSamplingFrequency;
    }

    void setSamplingFrequency(sampling_frequency_t frequency) {
        mSamplingFrequency = frequency;        
        mLoop.setDelay(std::chrono::duration<sampling_frequency_t>(1.0/mSamplingFrequency));
        mBuffer.reset();
    }
};

ConnectionStatsChannel::ConnectionStatsChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::ConnectionStats),
    mImpl(std::make_unique<Impl>(device)){}

ConnectionStatsChannel::~ConnectionStatsChannel(){}


ConnectionStatsChannel::length_listener_ptr
ConnectionStatsChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
}

auto ConnectionStatsChannel::readData(data_offset_t offset, data_length_t length) const -> data_container{
    return mImpl->readData(offset, length);
}

data_length_t ConnectionStatsChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t ConnectionStatsChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> ConnectionStatsChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t ConnectionStatsChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void ConnectionStatsChannel::setSamplingFrequency(sampling_frequency_t frequency) {
    mImpl->setSamplingFrequency(frequency);
}

}
