#ifndef EMULATION_CHANNEL_H
#define EMULATION_CHANNEL_H

#include <vector>
#include <memory>
#include "lib_export.h"
#include "common_types.h"
#include "channels/info/channel_info.h"
#include "event_listener.h"

namespace Neuro {

struct EmulationSine {
	double AmplitudeV{ 0.0 };
	double FrequencyHz{ 0.0 };
	double PhaseShiftRad{ 0.0 };
};

class EmulationChannel final {
public:
	using LengthCallbackType = std::function<void(data_length_t)>;
	using LengthListenerType = ListenerPtr<void, data_length_t>;
	using DataType = double;
	using DataContainer = std::vector<DataType>;

	EmulationChannel(const std::vector<EmulationSine> &components, sampling_frequency_t sampling_frequency, data_length_t initial_length);
	EmulationChannel(const EmulationChannel &) = delete;
	EmulationChannel& operator=(const EmulationChannel &) = delete;
	EmulationChannel(EmulationChannel &&) noexcept;
	EmulationChannel& operator=(EmulationChannel &&) noexcept;
	~EmulationChannel();

	SDK_SHARED ChannelInfo& info() const noexcept;
	SDK_SHARED LengthListenerType subscribeLengthChanged(LengthCallbackType) noexcept;
	SDK_SHARED DataContainer readData(data_offset_t, data_length_t) const;
	SDK_SHARED data_length_t totalLength() const noexcept;
	SDK_SHARED sampling_frequency_t samplingFrequency() const noexcept;
	SDK_SHARED void startTimer();
	SDK_SHARED void stopTimer();
	SDK_SHARED void reset();

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}
#endif // EMULATION_CHANNEL_H
