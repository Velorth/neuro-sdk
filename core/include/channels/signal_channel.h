#ifndef SIGNAL_CHANNEL_H
#define SIGNAL_CHANNEL_H

#include "base_channel.h"
#include "filter/digital_filter.h"
#include "lib_export.h"

namespace Neuro {

class SDK_SHARED SignalChannel final : public BaseChannel<signal_sample_t> {
public:    
    class Impl;

    SignalChannel(std::shared_ptr<Device>,
                  const ChannelInfo & = ChannelInfo::Signal());

    SignalChannel(std::shared_ptr<Device>,
                  std::vector<DSP::DigitalFilterPtr<signal_sample_t>> &&,
                  const ChannelInfo & = ChannelInfo::Signal());

    ~SignalChannel();

    length_listener_ptr subscribeLengthChanged(length_callback_t) noexcept override ;
    data_container readData(data_offset_t, data_length_t) const override;
    data_length_t totalLength() const noexcept override;
    data_length_t bufferSize() const noexcept override;
    std::weak_ptr<Device> underlyingDevice() const noexcept override;
    sampling_frequency_t samplingFrequency() const noexcept override;
    void setSamplingFrequency(sampling_frequency_t) override;

private:
    std::unique_ptr<Impl> mImpl;
};

}

#endif // SIGNAL_CHANNEL_H
