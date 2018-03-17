#ifndef BASE_CHANNEL_H
#define BASE_CHANNEL_H

#include <memory>
#include <vector>
#include "common_types.h"
#include "channel_info.h"

namespace Neuro {

class Device;

template <typename DataType>
class BaseChannel {
public:
    using data_container = std::vector<DataType>;

    BaseChannel(ChannelInfo &&info) noexcept : mInfo(std::move(info)) {}
    BaseChannel(const ChannelInfo &info) : mInfo(info) {}
    virtual ~BaseChannel() = default;

    ChannelInfo& info() noexcept {
        return mInfo;
    }

    virtual data_container readData(data_offset_t, data_length_t) const = 0;
    virtual data_length_t totalLength() const noexcept = 0;
    virtual data_length_t availableLength() const noexcept = 0;
    virtual sampling_frequency_t samplingFrequency() const noexcept = 0;
    virtual void setSamplingFrequency(sampling_frequency_t) = 0;
    virtual std::weak_ptr<Device> underlyingDevice() const noexcept = 0;

protected:
    ChannelInfo mInfo;
};

}

#endif // BASE_CHANNEL_H
