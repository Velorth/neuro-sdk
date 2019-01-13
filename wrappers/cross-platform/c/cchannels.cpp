extern "C"
{
#include "cchannels.h"
}

#include <memory>
#include "cchannel-helper.h"
#include "sdk_error.h"

using AnyChannelWrapperPtr = std::shared_ptr<AnyChannelWrapper>;

void AnyChannel_delete(AnyChannel *channel) {
	const auto anyChannel = reinterpret_cast<AnyChannelWrapperPtr *>(channel);
	delete anyChannel;
}

int AnyChannel_get_info(AnyChannel * channel, ChannelInfo * out_info) {
	auto& anyChannel = *reinterpret_cast<AnyChannelWrapperPtr *>(channel);
	return getChannelInfo(*anyChannel, out_info);
}

int AnyChannel_get_sampling_frequency(AnyChannel* channel, float* out_frequency) {
	auto& anyChannel = *reinterpret_cast<AnyChannelWrapperPtr *>(channel);
	return readSamplingFrequency(*anyChannel, out_frequency);
}

int AnyChannel_add_length_callback(AnyChannel *channel, void(*callback)(AnyChannel *, size_t), ListenerHandle* handle) {
	auto& anyChannel = *reinterpret_cast<AnyChannelWrapperPtr *>(channel);
	try {
		auto listener = anyChannel->subscribeLengthChanged([channel, callback](size_t new_length) {
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

int AnyChannel_get_total_length(AnyChannel* channel, size_t* out_length) {
	auto& anyChannel = *reinterpret_cast<AnyChannelWrapperPtr *>(channel);
	return readTotalLength(*anyChannel, out_length);
}




using IntChannelWrapperPtr = std::shared_ptr<DataChannelWrapper<int>>;

int IntChannel_read_data(IntChannel *channel, size_t offset, size_t length, int *out_buffer) {
	auto& intChannel = *reinterpret_cast<IntChannelWrapperPtr *>(channel);
	return readChannelData(*intChannel, offset, length, out_buffer);
}



using DoubleChannelWrapperPtr = std::shared_ptr<DataChannelWrapper<double>>;

int DoubleChannel_read_data(DoubleChannel *channel, size_t offset, size_t length, double *out_buffer) {
	auto& doubleChannel = *reinterpret_cast<DoubleChannelWrapperPtr *>(channel);
	return readChannelData(*doubleChannel, offset, length, out_buffer);
}