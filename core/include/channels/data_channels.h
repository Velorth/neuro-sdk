#ifndef BASE_CHANNEL_H
#define BASE_CHANNEL_H

#include <memory>
#include <vector>
#include <functional>
#include "common_types.h"
#include "info/channel_info.h"
#include "event_listener.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class Device;

class SDK_SHARED CommonChannelInterface {
protected:
    using length_callback_t = std::function<void(data_length_t)>;
    using length_listener_ptr = ListenerPtr<void, data_length_t>;

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

    virtual length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept = 0;
    virtual data_length_t totalLength() const noexcept = 0;
    virtual data_length_t bufferSize() const noexcept = 0;
    virtual sampling_frequency_t samplingFrequency() const noexcept = 0;

private:
    ChannelInfo mInfo;
};

template <typename DataType>
class BaseChannel : public CommonChannelInterface{
protected:
    using data_container = std::vector<DataType>;

    explicit BaseChannel(ChannelInfo &&channel_info) noexcept :
		CommonChannelInterface(std::move(channel_info)){}

	explicit BaseChannel(const ChannelInfo &channel_info) :
		CommonChannelInterface(channel_info) {}

	BaseChannel(const BaseChannel &) = default;
	BaseChannel(BaseChannel &&) = default;

	BaseChannel& operator=(const BaseChannel &) = default;
	BaseChannel& operator=(BaseChannel &&) = default;

public:
    virtual ~BaseChannel() = default;
    virtual data_container readData(data_offset_t, data_length_t) const = 0;
};

template <ChannelInfo::Type ChannelType>
using ChannelCircularBuffer = SafeBuffer<typename ChannelTraits<ChannelType>::DataType, ChannelTraits<ChannelType>::MemoryBufferSize>;

template <ChannelInfo::Type ChannelType, typename BufferType = ChannelCircularBuffer<ChannelType>>
class DataChannel final : public BaseChannel<typename ChannelTraits<ChannelType>::DataType> {
public:
	using ChannelTraits = ChannelTraits<ChannelType>;
	using ChannelDataType = typename ChannelTraits::DataType;
	using length_callback_t = CommonChannelInterface::length_callback_t;
	using length_listener_ptr = CommonChannelInterface::length_listener_ptr;
	using data_container = typename BaseChannel<ChannelDataType>::data_container;

	explicit DataChannel(const std::shared_ptr<Device> &device, ChannelInfo &&channel_info = ChannelTraits::defaultInfo()) noexcept :
		BaseChannel(std::move(channel_info)),
		mDevice(device),
		mDataListener(device->subscribeDataReceived<ChannelType>([=](auto...))){
		
	}

	DataChannel(const std::shared_ptr<Device> &device, ChannelInfo channel_info) : 
		DataChannel(device, std::move(channel_info)) {}

	length_listener_ptr subscribeLengthChanged(length_callback_t callback) noexcept override {
		return mBuffer.subscribeLengthChanged(callback);
	}

	data_length_t totalLength() const noexcept override {
		return mBuffer.totalLength();
	}

	data_length_t bufferSize() const noexcept override {
		return mBuffer.bufferSize();
	}

	sampling_frequency_t samplingFrequency() const noexcept {
		return getSamplingFrequency();
	}

	data_container readData(data_offset_t offset, data_length_t length) const override {
		return mBuffer.readFill(offset, length, ChannelDataType{});
	}

	std::weak_ptr<Device> underlyingDevice() const noexcept {
		return mDevice;
	}

private:
	std::shared_ptr<Device> mDevice;
	ChannelDataListenerType<ChannelType> mDataListener;
	BufferType mBuffer;

	constexpr sampling_frequency_t getSamplingFrequency() const {
		return ChannelTraits::SamplingFrequency;
	}
};

template<>
constexpr sampling_frequency_t DataChannel<ChannelInfo::Type::Signal>::getSamplingFrequency() const;

using SignalChannel = DataChannel<ChannelInfo::Type::Signal>;
using ResistanceChannel = DataChannel<ChannelInfo::Type::Resistance>;
using MemsChannel = DataChannel<ChannelInfo::Type::MEMS>; 
using RespirationChannel = DataChannel<ChannelInfo::Type::Respiration>;
using OrientationChannel = DataChannel<ChannelInfo::Type::Orientation>;

}
#endif // BASE_CHANNEL_H
