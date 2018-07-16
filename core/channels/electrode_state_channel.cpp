#include "gsl/gsl_assert"
#include "channels/channel_info.h"
#include "channels/electrode_state_channel.h"
#include "device/device.h"
#include "device/device_impl.h"
#include "device/param_values.h"
#include "signal/safe_buffer.h"
#include "loop.h"

namespace Neuro {

class ElectrodeStateChannel::Impl{
private:
    static constexpr const char *class_name = "ElectrodeStateChannel";
    static constexpr std::size_t BufferSize = 600; //10 minutes for 1 Hz sampling frequency
    static constexpr sampling_frequency_t DefaultFrequency = 1.f;
    std::shared_ptr<Device> mDevice;
    SafeBuffer<ElectrodeState, BufferSize> mBuffer;
    sampling_frequency_t mSamplingFrequency{DefaultFrequency};
    Loop<void(Impl*)> mLoop;

    void readElectrodeFunc(){
        auto electrodeState = ElectrodeState::Normal;
        try {
            electrodeState = mDevice->mImpl->isElectrodesAttached() ?
                        ElectrodeState::Normal :
                        ElectrodeState::Detached;
        }
        catch (std::runtime_error &) {
            electrodeState = getLastElectrodeState();
        }
        mBuffer.append({electrodeState});
    }

    ElectrodeState getLastElectrodeState(){
        if (mBuffer.totalLength()>0){
            auto lastState = mBuffer.readFill(mBuffer.totalLength()-1, 1, ElectrodeState::Normal);
            return lastState[0];
        }
        return ElectrodeState::Normal;
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
        mDevice(device),
        mLoop(&Impl::readElectrodeFunc, std::chrono::duration<sampling_frequency_t>(1.0/mSamplingFrequency), this){
        Expects(mDevice != nullptr);
        Expects(checkHasChannel(*device, ChannelInfo::ElectrodesState()));
        checkAdcInput();
    }

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept {
        try{
            return mBuffer.subscribeLengthChanged(callback);
        }
        catch(...){
            return nullptr;
        }
    }

    ElectrodeStateChannel::data_container readData(data_offset_t offset, data_length_t length) const {
        return mBuffer.readFill(offset, length, ElectrodeState::Normal);
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

ElectrodeStateChannel::ElectrodeStateChannel(std::shared_ptr<Device> device):
    BaseChannel(ChannelInfo::ElectrodesState()),
    mImpl(std::make_unique<Impl>(device)){

}

ElectrodeStateChannel::~ElectrodeStateChannel(){}

ElectrodeStateChannel::length_listener_ptr
ElectrodeStateChannel::subscribeLengthChanged(length_callback_t callback) noexcept {
    return mImpl->subscribeLengthChanged(callback);
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
