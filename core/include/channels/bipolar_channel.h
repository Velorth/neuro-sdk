#ifndef BIPOLAR_CHANNEL_H
#define BIPOLAR_CHANNEL_H

#include <memory>
#include <string>
#include <vector>
#include "info/channel_info.h"
#include "event_notifier.h"

namespace Neuro {

template <typename Channel>
std::string bipolarChannelInfo(const Channel &first, const Channel &second) {
	return ChannelInfo(ChannelInfo::Type::Custom, first.info().getName() + std::string("-") + second.info().getName());
}

template <typename Channel, typename DataT>
class BipolarChannel final {
public:
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataType = DataT;
	using DataContainer = std::vector<DataType>;

	BipolarChannel(std::shared_ptr<Channel> first, std::shared_ptr<Channel> second):
		mInfo(bipolarChannelInfo(*first, *second)),
		mFirst(std::move(first)),
		mSecond(std::move(second)) {	
		checkPreconditions();

		mFirst->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
		mSecond->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
	}
	
	ChannelInfo& info() noexcept {
		return mInfo;
	}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept {
		return mLengthNotifier.addListener(callback);
	}

	data_length_t totalLength() const noexcept {
		return minimumLength();
	}

	sampling_frequency_t samplingFrequency() const noexcept {
		return mFirst->samplingFrequency();
	}

	DataContainer readData(data_offset_t offset, data_length_t length) const {
		auto firstData = mFirst->readData(offset, length);
		auto secondData = mSecond->readData(offset, length);
		if (firstData.size() != secondData.size()) {
			throw std::runtime_error("Channels returned data arrays of different lengthes");
		}
		std::transform(firstData.begin(), firstData.end(), secondData.begin(), std::minus<DataType>());
		return firstData;
	}

private:
	ChannelInfo mInfo;
	Notifier<void, data_length_t> mLengthNotifier;
	std::shared_ptr<Channel> mFirst;
	std::shared_ptr<Channel> mSecond;
	LengthListenerType mFirstLengthListener;
	LengthListenerType mSecondLengthListener;

	void checkPreconditions() const {
		if (mFirst->samplingFrequency() != mSecond->samplingFrequency()) {
			throw std::runtime_error("Channels have different sampling frequencies");
		}
		if (mFirst->totalLength() != mSecond->totalLength()) {
			throw std::runtime_error("Channels have different lengthes");
		}
	}

	data_length_t minimumLength() const noexcept {
		return std::min(mFirst->totalLength(), mSecond->totalLength);
	}

	void onDataLengthChanged() {
		mLengthNotifier.notifyAll(totalLength());
	}
};

template <typename Channel>
auto make_bipolar(Channel&& first, Channel&& second) {
	using DataType = typename Channel::DataType;
	return BipolarChannel<Channel, DataType>(
		std::make_shared<Channel>(std::forward<Channel>(first)),
		std::make_shared<Channel>(std::forward<Channel>(second))
		);
}

template <typename ChannelPtr>
auto make_bipolar_from_ptrs(ChannelPtr&& first, ChannelPtr&& second) {
	using Channel = typename ChannelPtr::element_type;
	using DataType = typename Channel::DataType;
	return BipolarChannel<Channel, DataType>(
		std::forward<Channel>(first), 
		std::forward<Channel>(second)
		);
}

}

#endif