#ifndef BATTERY_CHANNEL_H
#define BATTERY_CHANNEL_H

#include <memory>
#include "base_channel.h"

namespace Neuro {

class BatteryChannel final : public BaseChannel<int> {
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

public:
    BatteryChannel(std::shared_ptr<Device>);
    ~BatteryChannel();

    void setLengthChangedCallback(length_changed_callback_t) noexcept override;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
    std::weak_ptr<Device> underlyingDevice() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    void setSamplingFrequency(sampling_frequency_t) override;
};

}

#endif // BATTERY_CHANNEL_H
