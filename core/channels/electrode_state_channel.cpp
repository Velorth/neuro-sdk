#include <thread>
#include <mutex>
#include <condition_variable>
#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/electrode_state_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class ElectrodeStateChannel::Impl{
private:
    static constexpr std::size_t BufferSize = 600; //10 minutes for 1 Hz sampling frequency
    static constexpr sampling_frequency_t DefaultFrequency = 1.f;
    std::shared_ptr<Device> mDevice;
    SafeBuffer<ElectrodeState, BufferSize> mBuffer;
    sampling_frequency_t mSamplingFrequency{DefaultFrequency};
    std::mutex mWaitElectrodeMutex;
    std::mutex mBufferMutex;
    std::condition_variable mWaitElectrodeCondition;
    std::thread mReadElectrodeThread;

    void readElectrodeFunc(){
        std::unique_lock<std::mutex> waitElectrodeLock(mWaitElectrodeMutex);
        do {
            auto electrodeState = ElectrodeState::Attached;
            try {
                electrodeState = mDevice->mImpl->isElectrodesAttached() ?
                            ElectrodeState::Attached :
                            ElectrodeState::Detached;
            }
            catch (std::runtime_error &) {
                electrodeState = getLastElectrodeState();
            }
            std::unique_lock<std::mutex> bufferLock(mBufferMutex);
            mBuffer.append({electrodeState});
        }while(mWaitElectrodeCondition.wait_for(waitElectrodeLock, std::chrono::duration<sampling_frequency_t>(1.f/mSamplingFrequency)) == std::cv_status::timeout);
    }

    ElectrodeState getLastElectrodeState(){
        if (mBuffer.availableLength()>0){
            auto lastState = mBuffer.readFill(mBuffer.totalLength()-1, 1, ElectrodeState::Attached);
            return lastState[0];
        }
        return ElectrodeState::Attached;
    }

    void checkAdcInput(){
        if (checkHasParameter(*mDevice, Parameter::ADCInputState)
                && getParameterAccess(*mDevice, Parameter::ADCInputState) == ParamAccess::ReadWrite){
            switchAdcInput();
        }
    }

    void switchAdcInput(){
        if (!mDevice->setParam<Parameter::ADCInputState>(ADCInput::Resistance))
            throw std::runtime_error("Unable select proper ADC input for electrode state tracking");
    }

public:
    Impl(std::shared_ptr<Device> device) :
        mDevice(device){
        Expects(mDevice != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::ElectrodesState));
        checkAdcInput();
        mReadElectrodeThread = std::thread([=](){ readElectrodeFunc(); });
        Ensures(mReadElectrodeThread.joinable());
    }

    ~Impl(){
        std::unique_lock<std::mutex> waitElectrodeThreadLock(mWaitElectrodeMutex);
        mWaitElectrodeCondition.notify_all();
        try{
            mReadElectrodeThread.join();
        }
        catch (std::system_error &){
#ifndef NDEBUG
            Ensures(false);
#endif
        }
    }

    void setLengthChangedCallback(length_changed_callback_t callback) noexcept {

    }

    ElectrodeStateChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        return mBuffer.readFill(offset, length, ElectrodeState::Attached);
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
        std::unique_lock<std::mutex> bufferLock(mBufferMutex);
        mSamplingFrequency = frequency;
        mBuffer.reset();
    }
};

ElectrodeStateChannel::ElectrodeStateChannel(std::shared_ptr<Device> device):
    BaseChannel(ChannelInfo::ElectrodesState),
    mImpl(std::make_unique<Impl>(device)){

}

ElectrodeStateChannel::~ElectrodeStateChannel(){}

void ElectrodeStateChannel::setLengthChangedCallback(length_changed_callback_t callback) noexcept {
    mImpl->setLengthChangedCallback(callback);
}

ElectrodeStateChannel::data_container ElectrodeStateChannel::readData(data_offset_t offset, data_length_t length) const {
    return mImpl->readData(offset, length);
}

data_length_t ElectrodeStateChannel::totalLength() const noexcept {
    return mImpl->totalLength();
}

data_length_t ElectrodeStateChannel::bufferSize() const noexcept {
    return mImpl->bufferSize();
}

std::weak_ptr<Device> ElectrodeStateChannel::underlyingDevice() const noexcept {
    return mImpl->underlyingDevice();
}

sampling_frequency_t ElectrodeStateChannel::samplingFrequency() const noexcept {
    return mImpl->samplingFrequency();
}

void ElectrodeStateChannel::setSamplingFrequency(sampling_frequency_t frequency) {
    mImpl->setSamplingFrequency(frequency);
}

}
