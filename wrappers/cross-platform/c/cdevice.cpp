extern "C"
{
#include "cdevice.h"
#include "sdk_error.h"
}
#include <algorithm>
#include "device/device.h"

extern std::string sdk_last_error;

template<Neuro::Parameter Param>
bool setDeviceParam(const std::unique_ptr<Neuro::Device> &device, ParamValueType param_value) {
	auto value = getSdkParamValue<Param::Type>(param_value);
	return device->setParam<Param>(value);
}

ret_code device_connect(Device *device_ptr) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->connect();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

ret_code device_disconnect(Device *device_ptr) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->connect();
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

void device_delete(Device *device_ptr) {
	const auto device_raw = reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	delete device_raw;
}

ret_code device_available_channels(const Device *device_ptr, ChannelInfoArray *channel_info_array) {
	auto& device = *reinterpret_cast<const std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto channels = device->channels();
		channel_info_array->info_count = channels.size();
		const auto memorySize = channel_info_array->info_count * sizeof(ChannelInfo);
		channel_info_array->info_array = reinterpret_cast<ChannelInfo *>(malloc(memorySize));
		std::transform(channels.begin(), channels.end(), channel_info_array->info_array, [](const auto& channel_info) {
			ChannelInfo info;
			strcpy(info.name, channel_info.getName().c_str());
			info.type = static_cast<ChannelType>(channel_info.getType());
			info.index = channel_info.getIndex();
			return info;
		});
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

ret_code device_available_commands(const Device *device_ptr, CommandArray *) {
	throw;
}

ret_code device_available_parameters(const Device *device_ptr, ParamInfoArray *) {
	throw;
}

ret_code device_execute(Device *device_ptr, Command cmd) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		if (device->execute(static_cast<Neuro::Command>(cmd))) {
			return SDK_NO_ERROR;
		}
		else {
			sdk_last_error = "Unable to execute command";
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

ret_code device_subscribe_param_changed(Device* device_ptr, void(*callback)(Parameter)) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParamChangedCallback([callback](auto param) {
			if (callback != nullptr) {
				callback(static_cast<Parameter>(param));
			}
		});
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

ret_code device_read_param(Device *device_ptr, Parameter, ParamValueType *result) {
	throw;
}

ret_code device_set_param(Device *device_ptr, Parameter parameter, ParamValueType value) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		bool result{ false };
		switch (parameter) {
		case ParameterName: {
			result = setDeviceParam<Neuro::Parameter::Name>(device, value);
			break;
		}
		case ParameterState: {
			result = setDeviceParam<Neuro::Parameter::State>(device, value);
			break;
		}
		case ParameterAddress: {
			result = setDeviceParam<Neuro::Parameter::Address>(device, value);
			break;
		}
		case ParameterSerialNumber: {
			result = setDeviceParam<Neuro::Parameter::SerialNumber>(device, value);
			break;
		}
		case ParameterHardwareFilterState: {
			result = setDeviceParam<Neuro::Parameter::HardwareFilterState>(device, value);
			break;
		}
		case ParameterFirmwareMode: {
			result = setDeviceParam<Neuro::Parameter::FirmwareMode>(device, value);
			break;
		}
		case ParameterSamplingFrequency: {
			result = setDeviceParam<Neuro::Parameter::SamplingFrequency>(device, value);
			break;
		}
		case ParameterGain: {
			result = setDeviceParam<Neuro::Parameter::Gain>(device, value);
			break;
		}
		case ParameterOffset: {
			result = setDeviceParam<Neuro::Parameter::Offset>(device, value);
			break;
		}
		case ParameterExternalSwitchState: {
			result = setDeviceParam<Neuro::Parameter::ExternalSwitchState>(device, value);
			break;
		}
		case ParameterADCInputState: {
			result = setDeviceParam<Neuro::Parameter::ADCInputState>(device, value);
			break;
		}
		case ParameterAccelerometerSens: {
			result = setDeviceParam<Neuro::Parameter::AccelerometerSens>(device, value);
			break;
		}
		case ParameterGyroscopeSens: {
			result = setDeviceParam<Neuro::Parameter::GyroscopeSens>(device, value);
			break;
		}
		case ParameterStimulatorState: {
			result = setDeviceParam<Neuro::Parameter::StimulatorState>(device, value);
			break;
		}
		case ParameterMotionAssistantState: {
			result = setDeviceParam<Neuro::Parameter::MotionAssistantState>(device, value);
			break;
		}
		case ParameterStimulatorParamPack: {
			result = setDeviceParam<Neuro::Parameter::StimulatorParamPack>(device, value);
			break;
		}
		case ParameterMotionAssistantParamPack: {
			result = setDeviceParam<Neuro::Parameter::MotionAssistantParamPack>(device, value);
			break;
		}
		default: {
			sdk_last_error = "Unknown parameter type";
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		}
		if (result) {
			return SDK_NO_ERROR;
		}
		else {
			sdk_last_error = "Unable to set parameter";
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
	}
	catch (std::exception &e) {
		sdk_last_error = e.what();
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

