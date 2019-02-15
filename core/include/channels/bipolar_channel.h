#ifndef BIPOLAR_CHANNEL_H
#define BIPOLAR_CHANNEL_H

#include <memory>
#include <string>
#include <vector>
#include "info/channel_info.h"
#include "event_notifier.h"

namespace Neuro {

template <typename Channel>
ChannelInfo bipolarChannelInfo(const Channel &first, const Channel &second) {
	return ChannelInfo(ChannelInfo::Type::Custom, first.info().getName() + std::string("-") + second.info().getName());
}

template <typename Channel>
class BipolarChannel final {
public:
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataType = typename Channel::DataType;
	using DataContainer = std::vector<DataType>;

	BipolarChannel(std::shared_ptr<Channel> first, std::shared_ptr<Channel> second):
		mInfo(bipolarChannelInfo(*first, *second)),
		mFirst(std::move(first)),
		mSecond(std::move(second)) {	
		checkPreconditions();

		mFirstLengthListener = mFirst->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
		mSecondLengthListener = mSecond->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
	}

	BipolarChannel(const BipolarChannel &rhs) :
		mInfo(rhs.mInfo),
		mFirst(rhs.mFirst),
		mSecond(rhs.mSecond) {
		mFirstLengthListener = mFirst->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
		mSecondLengthListener = mSecond->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
	}

	BipolarChannel(BipolarChannel &&rhs) noexcept(false) : 
		mInfo(std::move(rhs.mInfo)),
		mFirst(std::move(rhs.mFirst)),
		mSecond(std::move(rhs.mSecond)){
		rhs.mFirstLengthListener = nullptr;
		rhs.mSecondLengthListener = nullptr;
		mFirstLengthListener = mFirst->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
		mSecondLengthListener = mSecond->subscribeLengthChanged([=](data_length_t) { onDataLengthChanged(); });
	};

	~BipolarChannel() = default;

	BipolarChannel& operator=(BipolarChannel rhs) {
		swap(rhs);
		return *this;
	}

	ChannelInfo& info() noexcept {
		return mInfo;
	}

	const ChannelInfo& info() const noexcept {
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
		std::transform(firstData.begin(), firstData.end(), secondData.begin(), firstData.begin(), std::minus<DataType>());
		return firstData;
	}

	void swap(BipolarChannel &rhs) noexcept {
		using std::swap;
		swap(mInfo, rhs.mInfo);
		swap(mLengthNotifier, rhs.mLengthNotifier);
		swap(mFirst, rhs.mFirst);
		swap(mSecond, rhs.mSecond);
		swap(mFirstLengthListener, rhs.mFirstLengthListener);
		swap(mSecondLengthListener, rhs.mSecondLengthListener);
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
		return std::min(mFirst->totalLength(), mSecond->totalLength());
	}

	void onDataLengthChanged() {
		mLengthNotifier.notifyAll(totalLength());
	}
};

template <typename Channel>
void swap(BipolarChannel<Channel> &lhs, BipolarChannel<Channel> &rhs) noexcept {
	lhs.swap(rhs);
}

template <typename Channel>
auto make_bipolar(Channel&& first, Channel&& second) {
	return BipolarChannel<Channel>(
		std::make_shared<Channel>(std::forward<Channel>(first)),
		std::make_shared<Channel>(std::forward<Channel>(second))
		);
}

template <typename ChannelPtr>
auto make_bipolar_from_ptrs(ChannelPtr&& first, ChannelPtr&& second) {
	using Channel = typename std::remove_reference_t<ChannelPtr>::element_type;
	return BipolarChannel<Channel>(
		std::forward<ChannelPtr>(first), 
		std::forward<ChannelPtr>(second)
		);
}

}

#endif