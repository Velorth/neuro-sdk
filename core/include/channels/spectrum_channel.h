#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <memory>
#include "base_channel.h"

namespace Neuro {

class SpectrumChannel final : public BaseChannel<double> {
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

public:
    SpectrumChannel(std::shared_ptr<BaseChannel<double>>);
    ~SpectrumChannel();

    length_listener_ptr subscribeLengthChanged(length_callback_t) noexcept override;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
    std::weak_ptr<Device> underlyingDevice() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    void setSamplingFrequency(sampling_frequency_t) override;
    double hzPerSpectrumSample() const noexcept;
};

}
#endif // SPECTRUM_CHANNEL_H
