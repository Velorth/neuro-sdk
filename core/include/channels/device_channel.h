#ifndef DATA_CHANNEL_H
#define DATA_CHANNEL_H

#include "channels/data_channel.h"

namespace Neuro {

class Device;

template <ChannelInfo::Type ChannelType, typename BufferType = typename ChannelTraits<ChannelType>::BufferType>
class DeviceChannel final : public DataChannel<ChannelDataType<ChannelType>> {
public:
	using ChannelTraits = ChannelTraits<ChannelType>;
	using DataType = ChannelDataType<ChannelType>;
	using DataListenerType = ChannelDataListenerType<ChannelType>;
	using DataCallbackType = ChannelDataCallbackFunctionType<ChannelType>;
	using LengthCallbackType = CommonChannelInterface::LengthCallbackType;
	using LengthListenerType = CommonChannelInterface::LengthListenerPtr;
	using DataContainer = typename DataChannel<DataType>::DataContainer;
	using DevicePtr = std::shared_ptr<Device>;
	using DeviceWeakPtr = std::weak_ptr<Device>;

	explicit DeviceChannel(const DevicePtr &device, ChannelInfo &&channel_info = ChannelTraits::defaultInfo()) noexcept :
		DataChannel<DataType>(std::move(channel_info)),
		mDevice(device),
		mDataListener(device->subscribeDataReceived<ChannelType>(mDataCallaback)) {}

	DeviceChannel(const DevicePtr &device, const ChannelInfo &channel_info) :
		DataChannel<DataType>(channel_info),
		mDevice(device),
		mDataListener(device->subscribeDataReceived<ChannelType>(mDataCallaback)) {}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept override {
		return mBuffer.subscribeLengthChanged(callback);
	}

	data_length_t totalLength() const noexcept override {
		return mBuffer.totalLength();
	}

	data_length_t bufferSize() const noexcept {
		return mBuffer.bufferSize();
	}

	sampling_frequency_t samplingFrequency() const noexcept override {
		return ChannelTraits::SamplingFrequency;
	}

	DataContainer readData(data_offset_t offset, data_length_t length) const override {
		return mBuffer.readFill(offset, length, DataType{});
	}

	DeviceWeakPtr underlyingDevice() const noexcept {
		return mDevice;
	}

private:
	BufferType mBuffer;
	DataCallbackType mDataCallaback{ [=](auto&&... data_args) {
		mBuffer.append(ChannelTraits::forwardData(std::forward<decltype(data_args)>(data_args)...));
	} };
	DevicePtr mDevice;
	DataListenerType mDataListener;
};

template<>
sampling_frequency_t DeviceChannel<ChannelInfo::Type::Signal>::samplingFrequency() const noexcept;

template<>
sampling_frequency_t DeviceChannel<ChannelInfo::Type::Resistance>::samplingFrequency() const noexcept;

}
#endif // DATA_CHANNEL_H
