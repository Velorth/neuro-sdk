#ifndef FILTERED_CHANNEL_H
#define FILTERED_CHANNEL_H

#include <memory>
#include "common_types.h"
#include "channels/info/channel_info.h"
#include "filter/digital_filter.h"
#include "signal/safe_buffer.h"

namespace Neuro {

template <typename Channel>
class FilteredChannel final {
public:
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataType = typename Channel::DataType;
	using DataContainer = std::vector<DataType>;
	using FilterPtr = std::unique_ptr<DSP::DigitalFilter<typename DataContainer::value_type>>;

	FilteredChannel(std::shared_ptr<Channel> source_channel, FilterPtr &&filter):
		mSourceChannel(source_channel),
		mFilter(std::move(filter)),
		mSourceLengthListener(mSourceChannel->subscribeLengthChanged([=](auto length) { syncBuffer(); })) {
		syncBuffer();
	}

	ChannelInfo& info() noexcept {
		return mSourceChannel->info();
	}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept {
		return mBuffer.subscribeLengthChanged(callback);
	}

	data_length_t totalLength() const noexcept {
		return mBuffer.totalLength();
	}

	sampling_frequency_t samplingFrequency() const noexcept {
		return mSourceChannel->samplingFrequency();
	}

	DataContainer readData(data_offset_t offset, data_length_t length) const {
		return mBuffer.readFill(offset, length, 0.0);
	}

	data_length_t bufferSize() const noexcept {
		return mBuffer.bufferSize();
	}

private:
	std::shared_ptr<Channel> mSourceChannel;
	FilterPtr mFilter;
	SafeBuffer<double, 15000> mBuffer;
	ListenerPtr<void, data_length_t> mSourceLengthListener;
		
	void syncBuffer() {
		auto bufferLength = mBuffer.totalLength();
		auto channelLength = mSourceChannel->totalLength();

		if (channelLength <= bufferLength) {
			return;
		}
				
		auto lengthDiff = channelLength - bufferLength;
		auto newData = mSourceChannel->readData(bufferLength, lengthDiff);
		std::vector<double> filtered(newData.size());
		std::transform(newData.begin(), newData.end(), filtered.begin(), [=](auto sample) { return mFilter->filter(sample); });
		mBuffer.append(filtered);
	}
};

}

#endif
