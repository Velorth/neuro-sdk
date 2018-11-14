#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <memory>
#include "base_channel.h"
#include "lib_export.h"

namespace Neuro {

class SDK_SHARED SpectrumChannel final : public BaseChannel<double> {
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

public:
    SpectrumChannel(std::shared_ptr<BaseChannel<double>>);
    ~SpectrumChannel();

    LengthListenerPtr subscribeLengthChanged(LengthCallbackType) noexcept override;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    double hzPerSpectrumSample() const noexcept;
};

}
#endif // SPECTRUM_CHANNEL_H
