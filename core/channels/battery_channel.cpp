#include <thread>
#include <condition_variable>
#include <mutex>
#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/battery_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "signal/safe_buffer.h"
#include "logger.h"

namespace Neuro {

class BatteryChannel::Impl {
private:
    static constexpr const char *class_name = "BatteryChannel";
    static constexpr std::size_t BufferSize = 360; //1 hour for 0.1 Hz sampling frequency
    static constexpr sampling_frequency_t DefaultFrequency = 0.1f;
    std::shared_ptr<Device> mDevice;
    SafeBuffer<int, BufferSize> mBuffer;
    sampling_frequency_t mSamplingFrequency{DefaultFrequency};
    std::mutex mWaitBatteryMutex;
    mutable std::mutex mBufferMutex;
    std::condition_variable mWaitBatteryCondition;
    std::thread mReadBatteryThread;

    void readBatteryFunc(){
        std::unique_lock<std::mutex> waitBatteryLock(mWaitBatteryMutex);
        do {
            auto batteryPercents = 0;
            try {
                batteryPercents = mDevice->mImpl->batteryChargePercents();
            }
            catch (std::runtime_error &) {
                batteryPercents = getLastBatteryValue();
            }
            std::unique_lock<std::mutex> bufferLock(mBufferMutex);
            mBuffer.append({batteryPercents});
            bufferLock.unlock();

        }while(mWaitBatteryCondition.wait_for(waitBatteryLock, std::chrono::duration<sampling_frequency_t>(1.f/mSamplingFrequency)) == std::cv_status::timeout);
    }

    int getLastBatteryValue(){
        if (mBuffer.totalLength()>0){
            auto lastCharge = mBuffer.readFill(mBuffer.totalLength()-1, 1, 0);
            return lastCharge[0];
        }
        return 0;
    }

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(mDevice != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::Battery));
        mReadBatteryThread = std::thread([=](){ readBatteryFunc(); });
        Ensures(mReadBatteryThread.joinable());
    }

    ~Impl(){
        std::unique_lock<std::mutex> waitBatteryThreadLock(mWaitBatteryMutex);
        mWaitBatteryCondition.notify_all();
        try{
            if (mReadBatteryThread.joinable())
                mReadBatteryThread.join();
        }
        catch (std::system_error &e){
            LOG_ERROR_V("Thread destruction error: %s", e.what());
        }
    }

    length_listener_ptr setLengthChangedCallback(length_callback_t callback) noexcept {
        try{
            return mBuffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    BatteryChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        std::unique_lock<std::mutex> bufferLock(mBufferMutex);
        return mBuffer.readFill(offset, length, 0);
    }

    data_length_t totalLength() const noexcept {
        std::unique_lock<std::mutex> bufferLock(mBufferMutex);
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
        std::unique_lock<std::mutex> bufferLock(mBufferMutex);
        mSamplingFrequency = frequency;
        mBuffer.reset();
    }
};

BatteryChannel::BatteryChannel(std::shared_ptr<Device> device) :
    BaseChannel(ChannelInfo::Battery),
    mImpl(std::make_unique<Impl>(device)){}

BatteryChannel::~BatteryChannel(){}


BatteryChannel::length_listener_ptr
BatteryChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->setLengthChangedCallback(callback);
}

auto BatteryChannel::readData(data_offset_t offset, data_length_t length) const -> data_container{
    return mImpl->readData(offset, length);
}

data_length_t BatteryChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t BatteryChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> BatteryChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t BatteryChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void BatteryChannel::setSamplingFrequency(sampling_frequency_t frequency) {
    mImpl->setSamplingFrequency(frequency);
}

}
