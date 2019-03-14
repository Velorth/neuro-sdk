extern "C"
{
#include "cfiltered-channel.h"
}

#include "channels/filtered_channel.h"
#include "cchannel-helper.h"
#include "sdk_error.h"

using DoubleChannelWrap = DataChannelWrapper<double>;
using DoubleChannelWrapPtr = std::shared_ptr<DoubleChannelWrap>;

using FilteredChannelType = Neuro::FilteredChannel<DoubleChannelWrap>;
using FilteredChannelWrap = SpecificChannelWrapper<FilteredChannelType>;
using FilteredChannelWrapPtr = std::shared_ptr<FilteredChannelWrap>;

FilteredDoubleChannel* create_FilteredDoubleChannel(DoubleChannel *double_channel, Filter *filters, size_t filter_count) {
	auto& channel = *reinterpret_cast<DoubleChannelWrapPtr *>(double_channel);
	try {
		const auto channelPtr = std::make_shared<FilteredChannelType>(channel, getCompoundFilter(filters, filter_count));
		return getCObjectPtr<FilteredDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int FilteredDoubleChannel_get_buffer_size(FilteredDoubleChannel* channel, size_t* out_buffer_size) {
	auto& signalChannel = *reinterpret_cast<FilteredChannelWrapPtr *>(channel);
	return readBufferSize(*signalChannel->channelPtr(), out_buffer_size);
}
