#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <memory>
#include "data_channel.h"
#include "lib_export.h"

namespace Neuro {

class SpectrumChannel final : public DataChannel<double> {
private:
	std::shared_ptr<DataChannel<double>> mSourceChannel;

public:
	SDK_SHARED SpectrumChannel(std::shared_ptr<DataChannel<double>>);

	SDK_SHARED LengthListenerPtr subscribeLengthChanged(LengthCallbackType) noexcept override;
	SDK_SHARED DataContainer readData(data_offset_t, data_length_t) const override;
	SDK_SHARED data_length_t totalLength() const noexcept override;
	SDK_SHARED sampling_frequency_t samplingFrequency() const noexcept override;
	SDK_SHARED std::size_t spectrumLength() const noexcept;
	SDK_SHARED double hzPerSpectrumSample() const noexcept;
};

}
#endif // SPECTRUM_CHANNEL_H
