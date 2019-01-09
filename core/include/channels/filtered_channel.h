#ifndef FILTERED_CHANNEL_H
#define FILTERED_CHANNEL_H

namespace Neuro {

template <typename Channel, typename DataT>
class FilteredChannel final {
public:
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataType = DataT;
	using DataContainer = std::vector<DataType>;

	FilteredChannel() {
	}

	LengthListenerType subscribeLengthChanged(LengthCallbackType callback) noexcept {
	}

	data_length_t totalLength() const noexcept {
	}

	sampling_frequency_t samplingFrequency() const noexcept {
	}

	DataContainer readData(data_offset_t offset, data_length_t length) const {
	}
};

}

#endif
