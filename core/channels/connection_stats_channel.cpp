#include <thread>
#include <condition_variable>
#include <mutex>
#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/connection_stats_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class ConnectionStatsChannel::Impl {
private:
    static constexpr std::size_t BufferSize = 600; //10 minutes for 1 Hz sampling frequency
    static constexpr sampling_frequency_t DefaultFrequency = 1.f;
    std::shared_ptr<Device> mDevice;
    SafeBuffer<int, BufferSize> mBuffer;
    sampling_frequency_t mSamplingFrequency{DefaultFrequency};
    std::mutex mWaitConnectionStatsMutex;
    mutable std::mutex mDeviceMutex;
    std::condition_variable mWaitConnectionStatsCondition;
    std::thread mReadConnectionStatsThread;

    void readConnectionStatsFunc(){
        for (;;){
            std::unique_lock<std::mutex> waitConnectionStatsLock(mWaitConnectionStatsMutex);

            auto waitTime = std::chrono::duration<sampling_frequency_t>(1.f/mSamplingFrequency);
            auto waitResult = mWaitConnectionStatsCondition.wait_for(waitConnectionStatsLock,
                                                                      waitTime);
            if (waitResult == std::cv_status::no_timeout){
                return;
            }
        }
    }

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(mDevice != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::ConnectionStats));
        mReadConnectionStatsThread = std::thread([=](){ readConnectionStatsFunc(); });
        Ensures(mReadConnectionStatsThread.joinable());
    }

    ~Impl(){
        //std::unique_lock<std::mutex> waitConnectionStatsThreadLock(mWaitConnectionStatsMutex);
        mWaitConnectionStatsCondition.notify_all();
        try{
            if (mReadConnectionStatsThread.joinable())
                mReadConnectionStatsThread.join();
        }
        catch (std::system_error &){
#ifndef NDEBUG
            Ensures(false);
#endif
        }
    }

    void setLengthChangedCallback(length_changed_callback_t callback) noexcept {

    }

    ConnectionStatsChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        std::unique_lock<std::mutex> bufferLock(mDeviceMutex);
        return mBuffer.readFill(offset, length, 0);
    }

    data_length_t totalLength() const noexcept {
        std::unique_lock<std::mutex> bufferLock(mDeviceMutex);
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
        std::unique_lock<std::mutex> bufferLock(mDeviceMutex);
        mSamplingFrequency = frequency;
        mBuffer.reset();
    }
};

ConnectionStatsChannel::ConnectionStatsChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::ConnectionStats),
    mImpl(std::make_unique<Impl>(device)){}

ConnectionStatsChannel::~ConnectionStatsChannel(){}


void ConnectionStatsChannel::setLengthChangedCallback(length_changed_callback_t callback) noexcept {
    mImpl->setLengthChangedCallback(callback);
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
