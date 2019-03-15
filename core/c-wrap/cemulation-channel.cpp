extern "C"
{
#include "cemulation-channel.h"
}

#include "channels/emulation_channel.h"
#include "cchannel-helper.h"

using EmulationChannelType = Neuro::EmulationChannel;
using EmulationChannelWrap = SpecificChannelWrapper<EmulationChannelType>;
using EmulationWrapPtr = std::shared_ptr<EmulationChannelWrap>;

EmulationDoubleChannel* create_EmulationDoubleChannel(EmulationSine *components, size_t components_count, float sampling_frequency, size_t initial_length) {
	try {
		std::vector<Neuro::EmulationSine> sines(components_count);
		std::transform(components, components + components_count, sines.begin(), [](auto component) {
			Neuro::EmulationSine sine;
			sine.AmplitudeV = component.AmplitudeV;
			sine.FrequencyHz = component.FrequencyHz;
			sine.PhaseShiftRad = component.PhaseShiftRad;
			return sine;
		});

		const auto channelPtr = std::make_shared<EmulationChannelType>(sines, sampling_frequency, initial_length);
		return getCObjectPtr<EmulationDoubleChannel>(channelPtr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

int EmulationDoubleChannel_start_timer(EmulationDoubleChannel* channel) {
	auto& emulationChannel = *reinterpret_cast<EmulationWrapPtr *>(channel);
	try {
		emulationChannel->channelPtr()->startTimer();
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

int EmulationDoubleChannel_stop_timer(EmulationDoubleChannel* channel) {
	auto& emulationChannel = *reinterpret_cast<EmulationWrapPtr *>(channel);
	try {
		emulationChannel->channelPtr()->stopTimer();
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

int EmulationDoubleChannel_reset(EmulationDoubleChannel* channel) {
	auto& emulationChannel = *reinterpret_cast<EmulationWrapPtr *>(channel);
	try {
		emulationChannel->channelPtr()->reset();
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

