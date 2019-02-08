extern "C"
{
#include "cbridge-channels.h"
}

#include "cchannel-helper.h"
#include "channels/info/channel_info.h"

struct BridgeDoubleChannelObj : DataChannelWrapper<double> {
	BridgeDoubleChannelObj(ChannelInfo info, 
		ReadDataFunc read_data_func,
		GetFrequencyFunc get_frequency_func,
		AddLengthCallbackFunc add_length_callback_func,
		GetTotalLengthFunc get_total_length_func):
		mChannelInfo(Neuro::ChannelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index)),
		mReadDataFunc(read_data_func),
		mGetFrequencyFunc(get_frequency_func),
		mAddLengthCallbackFunc(add_length_callback_func),
		mGetTotalLengthFunc(get_total_length_func){}

	friend void BridgeDoubleChannel_lengthChanged(BridgeDoubleChannel *, std::size_t);

	Neuro::ChannelInfo& info() noexcept override {
		return mChannelInfo;
	}

	const Neuro::ChannelInfo& info() const noexcept override {
		return mChannelInfo;
	}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept override {
		mAddLengthCallbackFunc(BridgeDoubleChannel_lengthChanged, &mListenHandle);
		return mLengthListener.addListener(callback);
	}

	Neuro::data_length_t totalLength() const noexcept override {
		std::size_t totalLength;
		if (mGetTotalLengthFunc(&totalLength) != SDK_NO_ERROR) {
			return 0;
		}
		return totalLength;
	}

	Neuro::sampling_frequency_t samplingFrequency() const noexcept override {
		float samplingFreq;
		if (mGetFrequencyFunc(&samplingFreq) != SDK_NO_ERROR) {
			return .0f;
		}
		return samplingFreq;
	}

	~BridgeDoubleChannelObj() override = default;

	DataContainer readData(const Neuro::data_offset_t offset, const Neuro::data_length_t length) const override {
		std::vector<double> channelData(length);
		if (mReadDataFunc(offset, length, channelData.data()) != SDK_NO_ERROR){
			throw std::runtime_error("Unable to read channel data");
		}
		return channelData;
	}

private:
	Neuro::ChannelInfo mChannelInfo;
	Neuro::Notifier<void, Neuro::data_length_t> mLengthListener {"BridgeDoubleChannel"};
	ListenerHandle mListenHandle;
	const ReadDataFunc mReadDataFunc;
	const GetFrequencyFunc mGetFrequencyFunc;
	const AddLengthCallbackFunc mAddLengthCallbackFunc;
	const GetTotalLengthFunc mGetTotalLengthFunc;
};

void BridgeDoubleChannel_lengthChanged(BridgeDoubleChannel *channel, std::size_t length) {
	auto& bridgeChannel = *reinterpret_cast<std::shared_ptr<BridgeDoubleChannelObj> *>(channel);
	bridgeChannel->mLengthListener.notifyAll(length);
}

BridgeDoubleChannel* create_BridgeDoubleChannel_info(ChannelInfo info,
	ReadDataFunc read_data_func, 
	GetFrequencyFunc get_frequency_func,
	AddLengthCallbackFunc add_length_callback_func,
	GetTotalLengthFunc get_total_length_func) {
	const auto bridgeObject = std::make_shared<BridgeDoubleChannelObj>(info,
		read_data_func, 
		get_frequency_func, 
		add_length_callback_func, 
		get_total_length_func);
	return getCObjectPtr<BridgeDoubleChannel>(bridgeObject);
}
