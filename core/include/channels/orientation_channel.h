#ifndef ANGLE_CHANNEL_H
#define ANGLE_CHANNEL_H

#include "base_channel.h"
#include "quaternion.h"

namespace Neuro {

class OrientationChannel final : public BaseChannel<Quaternion> {
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

public:
    OrientationChannel(std::shared_ptr<Device>);
    ~OrientationChannel();

    void setLengthChangedCallback(length_changed_callback_t) noexcept override ;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
    std::weak_ptr<Device> underlyingDevice() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    void setSamplingFrequency(sampling_frequency_t) override;
};

}

#endif // ANGLE_CHANNEL_H
