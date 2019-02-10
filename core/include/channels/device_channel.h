#ifndef DATA_CHANNEL_H
#define DATA_CHANNEL_H


#include <memory>
#include <vector>
#include <functional>
#include "common_types.h"
#include "info/channel_info.h"
#include "filter/digital_filter.h"

namespace Neuro {

class Device;

template <typename DataContainer>
struct ChannelStrategy {
	virtual ~ChannelStrategy() = default;
	virtual DataContainer processData(const DataContainer &) = 0;
};

template <typename DataContainer>
struct FilterStrategy final : public ChannelStrategy<DataContainer> {
	using FilterPtr = std::unique_ptr<DSP::DigitalFilter<typename DataContainer::value_type>>;

	explicit FilterStrategy(FilterPtr &&filter) : 
		mFilter(std::move(filter)){}

	DataContainer processData(const DataContainer &data) override {
		DataContainer filteredData;
		std::transform(
			data.begin(), 
			data.end(), 
			std::back_inserter(filteredData), 
			[=](const auto &sample) {return mFilter->filter(sample); }
		);
		return filteredData;
	}

private:
	FilterPtr mFilter;
};

template <typename DataContainer>
struct RawStrategy final : public ChannelStrategy<DataContainer> {
	DataContainer processData(const DataContainer &data) override {
		return data;
	}
};

template <ChannelInfo::Type ChannelType, typename DeviceType = Device>
class DeviceChannel final {
public:
	using DeviceChannelTraits = ChannelTraits<ChannelType>;
	using DataType = ChannelDataType<ChannelType>;
	using BufferType = typename DeviceChannelTraits::BufferType;
	using DataListenerType = ChannelDataListenerType<ChannelType>;
	using DataCallbackType = ChannelDataCallbackFunctionType<ChannelType>;
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataContainer = std::vector<DataType>;
	using DataStrategyPtr = std::unique_ptr<ChannelStrategy<DataContainer>>;
	using FilterPtr = std::unique_ptr<DSP::DigitalFilter<DataType>>;
	using DevicePtr = std::shared_ptr<DeviceType>;
	using DeviceWeakPtr = std::weak_ptr<DeviceType>;

	explicit DeviceChannel(const DevicePtr &device, ChannelInfo &&channel_info = DeviceChannelTraits::defaultInfo()):
		mInfo(std::move(channel_info)),
		mDevice(device),
		mDataListener(device->template subscribeDataReceived<ChannelType>(mDataCallaback, mInfo)) {}

	DeviceChannel(const DevicePtr &device, const ChannelInfo &channel_info) :
		mInfo(channel_info),
		mDevice(device),
		mDataListener(device->template subscribeDataReceived<ChannelType>(mDataCallaback, mInfo)) {}

	DeviceChannel(const DevicePtr &device, FilterPtr &&filter, ChannelInfo &&channel_info = DeviceChannelTraits::defaultInfo()):
		mInfo(std::move(channel_info)),
		mDataStrategy(std::make_unique<FilterStrategy<DataContainer>>(std::move(filter))),
		mDevice(device),
		mDataListener(device->template subscribeDataReceived<ChannelType>(mDataCallaback, mInfo)) {}

	DeviceChannel(const DevicePtr &device, FilterPtr &&filter, const ChannelInfo &channel_info) :
		mInfo(channel_info),
		mDataStrategy(std::make_unique<FilterStrategy<DataContainer>>(std::move(filter))),
		mDevice(device),
		mDataListener(device->template subscribeDataReceived<ChannelType>(mDataCallaback, mInfo)) {}

	ChannelInfo& info() noexcept {
		return mInfo;
	}

	const ChannelInfo& info() const noexcept {
		return mInfo;
	}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept {
		return mBuffer.subscribeLengthChanged(callback);
	}

	data_length_t totalLength() const noexcept {
		return mBuffer.totalLength();
	}

	data_length_t bufferSize() const noexcept {
		return mBuffer.bufferSize();
	}

	sampling_frequency_t samplingFrequency() const noexcept {
		return DeviceChannelTraits::SamplingFrequency;
	}

	DataContainer readData(data_offset_t offset, data_length_t length) const {
		return mBuffer.readFill(offset, length, DataType{});
	}

	DeviceWeakPtr underlyingDevice() const noexcept {
		return mDevice;
	}

private:
	ChannelInfo mInfo;
	DataStrategyPtr mDataStrategy{std::make_unique<RawStrategy<DataContainer>>()};
	BufferType mBuffer;
	DataCallbackType mDataCallaback{ [=](auto&&... data_args) {
		auto rawData = DeviceChannelTraits::forwardData(std::forward<decltype(data_args)>(data_args)...);
		mBuffer.append(mDataStrategy->processData(rawData));
	} };
	DevicePtr mDevice;
	DataListenerType mDataListener;
};

template<>
SDK_SHARED sampling_frequency_t DeviceChannel<ChannelInfo::Type::Signal>::samplingFrequency() const noexcept;

template<ChannelInfo::Type ChannelType, typename DeviceType, typename ...Args>
DeviceChannel<ChannelType, DeviceType> make_channel(const std::shared_ptr<DeviceType> &device, Args&&... args) {
	return DeviceChannel<ChannelType, DeviceType>(device, std::forward<Args>(args)...);
}

}
#endif // DATA_CHANNEL_H
