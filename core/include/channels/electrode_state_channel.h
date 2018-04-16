#ifndef ELECTRODE_STATE_CHANNEL_H
#define ELECTRODE_STATE_CHANNEL_H

#include "base_channel.h"

namespace Neuro {

enum class ElectrodeState {
    Attached,
    Detached
};

class ElectrodeStateChannel final : public BaseChannel<ElectrodeState> {
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

public:
    ElectrodeStateChannel(std::shared_ptr<Device>);
    ~ElectrodeStateChannel();

    length_listener_ptr subscribeLengthChanged(length_callback_t) noexcept override;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
    std::weak_ptr<Device> underlyingDevice() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    void setSamplingFrequency(sampling_frequency_t) override;
};

}

#endif // ELECTRODE_STATE_CHANNEL_H
