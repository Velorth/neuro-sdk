#ifndef BASE_CHANNEL_H
#define BASE_CHANNEL_H

#include <memory>
#include <vector>
#include <functional>
#include "common_types.h"
#include "info/channel_info.h"

namespace Neuro {

class SDK_SHARED CommonChannelInterface {
protected:
    using LengthCallbackType = std::function<void(data_length_t)>;
    using LengthListenerPtr = ListenerPtr<void, data_length_t>;

    CommonChannelInterface(ChannelInfo &&info) noexcept : mInfo(std::move(info)) {}
    CommonChannelInterface(const ChannelInfo &info) : mInfo(info) {}
	CommonChannelInterface(const CommonChannelInterface &) = default;
	CommonChannelInterface(CommonChannelInterface &&) = default;
    virtual ~CommonChannelInterface() = default;

	CommonChannelInterface& operator=(const CommonChannelInterface &) = default;
	CommonChannelInterface& operator=(CommonChannelInterface &&) = default;

public:
    ChannelInfo& info() noexcept {
        return mInfo;
    }

    virtual LengthListenerPtr subscribeLengthChanged(LengthCallbackType callback) noexcept = 0;
    virtual data_length_t totalLength() const noexcept = 0;
    virtual sampling_frequency_t samplingFrequency() const noexcept = 0;

private:
    ChannelInfo mInfo;
};

template <typename DataType>
class DataChannel : public CommonChannelInterface {
protected:
    using DataContainer = std::vector<DataType>;

    explicit DataChannel(ChannelInfo &&channel_info) noexcept :
		CommonChannelInterface(std::move(channel_info)){}

	explicit DataChannel(const ChannelInfo &channel_info) :
		CommonChannelInterface(channel_info) {}

	DataChannel(const DataChannel &) = default;
	DataChannel(DataChannel &&) = default;

	DataChannel& operator=(const DataChannel &) = default;
	DataChannel& operator=(DataChannel &&) = default;

public:
    virtual ~DataChannel() = default;
    virtual DataContainer readData(data_offset_t, data_length_t) const = 0;
};

}
#endif // BASE_CHANNEL_H
