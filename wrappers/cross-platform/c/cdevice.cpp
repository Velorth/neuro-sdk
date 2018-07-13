extern "C"
{
#include "cdevice.h"
#include "sdk_error.h"
}
#include <algorithm>
#include "device/device.h"

extern std::string sdk_last_error;

ret_code device_connect(Device *device_ptr) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
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
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
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
	const auto device_raw = reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	delete device_raw;
}

ret_code device_available_channels(const Device *device_ptr, ChannelInfoArray *channel_info_array) {
	auto& device = *reinterpret_cast<const Neuro::DeviceSharedPtr *>(device_ptr);
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

ret_code device_available_commands(const Device *device_ptr, CommandArray *command_array) {
	auto& device = *reinterpret_cast<const Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		auto commands = device->commands();
		command_array->cmd_array_size = commands.size();
		const auto memorySize = command_array->cmd_array_size * sizeof(Command);
		command_array->cmd_array = reinterpret_cast<Command *>(malloc(memorySize));
		std::transform(commands.begin(), commands.end(), command_array->cmd_array, [](auto command) {
			return static_cast<Command>(command);
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

ret_code device_available_parameters(const Device *device_ptr, ParamInfoArray *param_info_array) {
	auto& device = *reinterpret_cast<const Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		auto params = device->parameters();
		param_info_array->info_count = params.size();
		const auto memorySize = param_info_array->info_count * sizeof(ParameterInfo);
		param_info_array->info_array = reinterpret_cast<ParameterInfo *>(malloc(memorySize));
		std::transform(params.begin(), params.end(), param_info_array->info_array, [](const auto& param_info) {
			ParameterInfo info;
			info.parameter = static_cast<Parameter>(param_info.first);
			info.access = static_cast<ParamAccess>(param_info.second);
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

ret_code device_execute(Device *device_ptr, Command cmd) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
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
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
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

