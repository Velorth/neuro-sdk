#ifndef BASE_CHANNEL_H
#define BASE_CHANNEL_H

#include <memory>
#include <vector>
#include <functional>
#include "common_types.h"
#include "info/channel_info.h"
#include "event_listener.h"

namespace Neuro {

class Device;

class SDK_SHARED CommonChannelInterface {
public:
    using length_callback_t = std::function<void(data_length_t)>;
    using length_listener_ptr = ListenerPtr<void, data_length_t>;

    CommonChannelInterface(ChannelInfo &&info) noexcept : mInfo(std::move(info)) {}
    CommonChannelInterface(const ChannelInfo &info) : mInfo(info) {}
    virtual ~CommonChannelInterface() = default;

    ChannelInfo& info() noexcept {
        return mInfo;
    }

    virtual length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept = 0;
    virtual data_length_t totalLength() const noexcept = 0;
    virtual data_length_t bufferSize() const noexcept = 0;
    virtual sampling_frequency_t samplingFrequency() const noexcept = 0;
    virtual void setSamplingFrequency(sampling_frequency_t) = 0;
    virtual std::weak_ptr<Device> underlyingDevice() const noexcept = 0;

protected:
    ChannelInfo mInfo;
};

template <typename DataType>
class BaseChannel : public CommonChannelInterface{
public:
    using data_container = std::vector<DataType>;

    template <typename T>
    BaseChannel(T&& channel_info):CommonChannelInterface(std::forward<T>(channel_info)){}
    virtual ~BaseChannel() = default;

    virtual data_container readData(data_offset_t, data_length_t) const = 0;
};

}
#endif // BASE_CHANNEL_H
