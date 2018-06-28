#ifndef RESISTANCE_CHANNEL_H
#define RESISTANCE_CHANNEL_H

#include "base_channel.h"
#include "lib_export.h"

namespace Neuro {

class SDK_SHARED ResistanceChannel final : public BaseChannel<resistance_sample_t> {
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

public:
    ResistanceChannel(std::shared_ptr<Device>);
    ~ResistanceChannel();

    length_listener_ptr subscribeLengthChanged(length_callback_t) noexcept override ;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
    std::weak_ptr<Device> underlyingDevice() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    void setSamplingFrequency(sampling_frequency_t) override;
};

}
#endif // RESISTANCE_CHANNEL_H
