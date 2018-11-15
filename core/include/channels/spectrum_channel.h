#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <memory>
#include "data_channel.h"
#include "lib_export.h"

namespace Neuro {

class SDK_SHARED SpectrumChannel final : public DataChannel<double> {
private:
	std::shared_ptr<DataChannel<double>> mSourceChannel;

public:
    SpectrumChannel(std::shared_ptr<DataChannel<double>>);

    LengthListenerPtr subscribeLengthChanged(LengthCallbackType) noexcept override;
    DataContainer readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
	std::size_t spectrumLength() const noexcept;
    double hzPerSpectrumSample() const noexcept;
};

}
#endif // SPECTRUM_CHANNEL_H
