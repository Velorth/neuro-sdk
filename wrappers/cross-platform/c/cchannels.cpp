extern "C"
{
#include "cchannels.h"
}

#include "device/device.h"
#include "channels/device_channel.h"
#include "channels/spectrum_channel.h"
#include "cchannel-helper.h"
#include "event_notifier.h"
#include "sdk_error.h"

BatteryChannel* create_BatteryChannel(Device *device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = new std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>>(std::make_shared<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>>(device));
		return reinterpret_cast<BatteryChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

void BatteryChannel_delete(BatteryChannel *channel) {
	const auto batteryChannel = reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	delete batteryChannel;
}

int BatteryChannel_get_info(BatteryChannel * channel, ChannelInfo * out_info) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	return getChannelInfo(*batteryChannel, out_info);
}

int BatteryChannel_read_data(BatteryChannel *channel, size_t offset, size_t length, int *out_buffer) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	return readChannelData(*batteryChannel, offset, length, out_buffer);
}

int BatteryChannel_get_buffer_size(BatteryChannel* channel, size_t* out_buffer_size) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	return readBufferSize(*batteryChannel, out_buffer_size);
}

int BatteryChannel_get_sampling_frequency(BatteryChannel* channel, float* out_frequency) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	return readSamplingFrequency(*batteryChannel, out_frequency);
}

int BatteryChannel_add_length_callback(BatteryChannel *channel, void(*callback)(BatteryChannel *, size_t), ListenerHandle* handle) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	try {
		auto listener = batteryChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int BatteryChannel_get_total_length(BatteryChannel* channel, size_t* out_length) {
	auto& batteryChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Battery>> *>(channel);
	return readTotalLength(*batteryChannel, out_length);
}

SignalChannel* create_SignalChannel(Device* device_ptr) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		const auto channelPtr = new std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(std::make_shared<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(device));
		return reinterpret_cast<SignalChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalChannel* create_SignalChannel_info(Device *device_ptr, ChannelInfo info) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		const auto channelPtr = new std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(std::make_shared<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(device, channelInfo));
		return reinterpret_cast<SignalChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalChannel* create_SignalChannel_info_filters(Device *device_ptr, ChannelInfo info, Filter *filters, size_t filter_count) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		auto digitalFilters = getCompoundFilter(filters, filter_count);
		const auto channelPtr = new std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(std::make_shared<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(device, std::move(digitalFilters), channelInfo));
		return reinterpret_cast<SignalChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

SignalChannel* create_SignalChannel_filters(Device *device_ptr, Filter *filters, size_t filter_count) {
	auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		auto digitalFilters = getCompoundFilter(filters, filter_count);
		const auto channelPtr = new std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(std::make_shared<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>>(device, std::move(digitalFilters)));
		return reinterpret_cast<SignalChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

void SignalChannel_delete(SignalChannel* channel) {
	const auto signalChannel = reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	delete signalChannel;
}

int SignalChannel_get_info(SignalChannel * channel, ChannelInfo * out_info) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	return getChannelInfo(*signalChannel, out_info);
}

int SignalChannel_read_data(SignalChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	return readChannelData(*signalChannel, offset, length, out_buffer);
}

int SignalChannel_get_sampling_frequency(SignalChannel* channel, float* out_frequency) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	return readSamplingFrequency(*signalChannel, out_frequency);
}

int SignalChannel_add_length_callback(SignalChannel* channel, void(*callback)(SignalChannel *, size_t), ListenerHandle* handle) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	try {
		auto listener = signalChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int SignalChannel_get_total_length(SignalChannel* channel, size_t* out_length) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	return readTotalLength(*signalChannel, out_length);
}

int SignalChannel_get_buffer_size(SignalChannel* channel, size_t* out_buffer_size) {
	auto& signalChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Signal>> *>(channel);
	return readBufferSize(*signalChannel, out_buffer_size);
}

ResistanceChannel* create_ResistanceChannel_info(Device* device_ptr, ChannelInfo info) {
	try {
		auto device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
		Neuro::ChannelInfo channelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index);
		const auto channelPtr = new std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>>(std::make_shared<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>>(device, channelInfo));
		return reinterpret_cast<ResistanceChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

void ResistanceChannel_delete(ResistanceChannel* channel) {
	auto resistanceChannel = reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel);
	delete resistanceChannel;
}

int ResistanceChannel_get_info(ResistanceChannel* channel, ChannelInfo* out_info) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel);
	return getChannelInfo(*resistanceChannel, out_info);
}

int ResistanceChannel_read_data(ResistanceChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel);
	return readChannelData(*resistanceChannel, offset, length, out_buffer);
}

int ResistanceChannel_get_sampling_frequency(ResistanceChannel* channel, float* out_frequency) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel);
	return readSamplingFrequency(*resistanceChannel, out_frequency);
}

int ResistanceChannel_add_length_callback(ResistanceChannel* channel, void(*callback)(ResistanceChannel*, size_t),
	ListenerHandle* handle) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel); 
	try {
		auto listener = resistanceChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int ResistanceChannel_get_total_length(ResistanceChannel* channel, size_t* out_length) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel);
	return readTotalLength(*resistanceChannel, out_length);
}

int ResistanceChannel_get_buffer_size(ResistanceChannel* channel, size_t* out_buffer_size) {
	auto& resistanceChannel = *reinterpret_cast<std::shared_ptr<Neuro::DeviceChannel<Neuro::ChannelInfo::Type::Resistance>> *>(channel);
	return readBufferSize(*resistanceChannel, out_buffer_size);
}

SpectrumChannel* create_SpectrumChannel(BaseDoubleChannel *signal_channel) {
	auto& channel = *reinterpret_cast<std::shared_ptr<Neuro::DataChannel<double>> *>(signal_channel);
	try {
		const auto channelPtr = new std::shared_ptr<Neuro::SpectrumChannel>(std::make_shared<Neuro::SpectrumChannel>(channel));
		return reinterpret_cast<SpectrumChannel *>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}
void SpectrumChannel_delete(SpectrumChannel* channel) {
	const auto spectrumChannel = reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	delete spectrumChannel;
}
int SpectrumChannel_get_info(SpectrumChannel* channel, ChannelInfo* out_info) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	return getChannelInfo(*spectrumChannel, out_info);
}

int SpectrumChannel_read_data(SpectrumChannel* channel, size_t offset, size_t length, double* out_buffer) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	try {
		auto data = spectrumChannel->readData(offset, length);
		if (data.size() != spectrumChannel->spectrumLength()) {
			set_sdk_last_error("Read data length is not equal to spectrum length");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		std::copy(data.begin(), data.end(), out_buffer);
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int SpectrumChannel_get_sampling_frequency(SpectrumChannel* channel, float* out_frequency) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	return readSamplingFrequency(*spectrumChannel, out_frequency);
}

int SpectrumChannel_get_hz_per_spectrum_sample(SpectrumChannel* channel, double* out_step) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	try {
		*out_step = spectrumChannel->hzPerSpectrumSample();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int SpectrumChannel_add_length_callback(SpectrumChannel* channel, void(* callback)(SpectrumChannel*, size_t),
	ListenerHandle* handle) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	try {
		auto listener = spectrumChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
			if (callback != nullptr) callback(channel, new_length);
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle *>(new decltype(listener)(listener));
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int SpectrumChannel_get_total_length(SpectrumChannel* channel, size_t* out_length) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	return readTotalLength(*spectrumChannel, out_length);
}

int SpectrumChannel_get_spectrum_length(SpectrumChannel* channel, size_t* out_spectrum_length) {
	auto& spectrumChannel = *reinterpret_cast<std::shared_ptr<Neuro::SpectrumChannel> *>(channel);
	try {
		*out_spectrum_length = spectrumChannel->spectrumLength();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

struct BridgeDoubleChannelObj : Neuro::DataChannel<double> {
	BridgeDoubleChannelObj(ChannelInfo info, 
		ReadDataFunc read_data_func,
		GetFrequencyFunc get_frequency_func,
		AddLengthCallbackFunc add_length_callback_func,
		GetTotalLengthFunc get_total_length_func):
		DataChannel<double>(Neuro::ChannelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index)),
		mReadDataFunc(read_data_func),
		mGetFrequencyFunc(get_frequency_func),
		mAddLengthCallbackFunc(add_length_callback_func),
		mGetTotalLengthFunc(get_total_length_func){}

	friend void BridgeDoubleChannel_lengthChanged(BaseDoubleChannel *, std::size_t);

	LengthListenerPtr subscribeLengthChanged(LengthCallbackType callback) noexcept override {
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
	Neuro::Notifier<void, Neuro::data_length_t> mLengthListener {"BridgeDoubleChannel"};
	ListenerHandle mListenHandle;
	const ReadDataFunc mReadDataFunc;
	const GetFrequencyFunc mGetFrequencyFunc;
	const AddLengthCallbackFunc mAddLengthCallbackFunc;
	const GetTotalLengthFunc mGetTotalLengthFunc;
};

void BridgeDoubleChannel_lengthChanged(BaseDoubleChannel *channel, std::size_t length) {
	auto& bridgeChannel = *reinterpret_cast<std::shared_ptr<BridgeDoubleChannelObj> *>(channel);
	bridgeChannel->mLengthListener.notifyAll(length);
}

BaseDoubleChannel* create_BridgeDoubleChannel_info(ChannelInfo info,
	ReadDataFunc read_data_func, 
	GetFrequencyFunc get_frequency_func,
	AddLengthCallbackFunc add_length_callback_func,
	GetTotalLengthFunc get_total_length_func) {
	auto bridgeObject = new std::shared_ptr<BridgeDoubleChannelObj>(std::make_shared<BridgeDoubleChannelObj>(info,
		read_data_func, 
		get_frequency_func, 
		add_length_callback_func, 
		get_total_length_func));
	return reinterpret_cast<BaseDoubleChannel *>(bridgeObject);
}

void BridgeDoubleChannel_delete(BaseDoubleChannel *channel) {
	const auto bridgeChannel = reinterpret_cast<std::shared_ptr<BridgeDoubleChannelObj> *>(channel);
	delete bridgeChannel;
}
