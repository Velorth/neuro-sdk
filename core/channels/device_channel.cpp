#include "channels/device_channel.h"
#include "device/device.h"
#include "device/param_values.h"

namespace Neuro {

template<>
SDK_SHARED sampling_frequency_t DeviceChannel<ChannelInfo::Type::Signal>::samplingFrequency() const noexcept {
	try {
		auto frequency = mDevice->readParam<Parameter::SamplingFrequency>();
		return static_cast<float>(intValue(frequency));
	}
	catch (std::runtime_error &) {
		return 0.f;
	}
}

}
