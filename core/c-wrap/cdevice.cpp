#include "clistener.h"

extern "C"
{
#include "cdevice.h"
#include "sdk_error.h"
}
#include <algorithm>
#include "device/device.h"

int command_to_string(Command cmd, char* buffer, size_t buffer_length) {
	try {
		auto commandString = Neuro::to_string(static_cast<Neuro::Command>(cmd));
		if (commandString.size() > buffer_length) {
			set_sdk_last_error("Command string is greater than read buffer");
			return ERROR_EXCEPTION_WITH_MESSAGE;

		}
		strcpy(buffer, commandString.c_str());
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

int parameter_to_string(Parameter param, char* buffer, size_t buffer_length) {
	try {
		auto paramString = Neuro::to_string(static_cast<Neuro::Parameter>(param));
		if (paramString.size() > buffer_length) {
			set_sdk_last_error("Parameter string is greater than read buffer");
			return ERROR_EXCEPTION_WITH_MESSAGE;

		}
		strcpy(buffer, paramString.c_str());
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

int parameter_access_to_string(ParamAccess access, char* buffer, size_t buffer_length) {
	try {
		auto accessString = Neuro::to_string(static_cast<Neuro::ParamAccess>(access));
		if (accessString.size() > buffer_length) {
			set_sdk_last_error("ParamAccess string is greater than read buffer");
			return ERROR_EXCEPTION_WITH_MESSAGE;

		}
		strcpy(buffer, accessString.c_str());
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

Device* create_Device(DeviceInfo device_info) {
	Neuro::DeviceInfo deviceInfo;
	deviceInfo.Address = static_cast<Neuro::DeviceAddressType>(device_info.Address);
	deviceInfo.Name = device_info.Name;
	auto devicePtr = new std::shared_ptr<Neuro::Device>(new Neuro::Device(deviceInfo));
	return reinterpret_cast<Device *>(devicePtr);
}

ret_code device_connect(Device *device_ptr) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		device->connect();
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

ret_code device_disconnect(Device *device_ptr) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		device->disconnect();
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
		set_sdk_last_error(e.what());
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
		set_sdk_last_error(e.what());
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
		set_sdk_last_error(e.what());
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
			set_sdk_last_error("Unable to execute command");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch (...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

ret_code device_subscribe_param_changed(Device* device_ptr, void(*callback)(Device*, Parameter, void *), ListenerHandle *handle, void *user_data) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		auto listener = device->setParamChangedCallback([device_ptr, callback, user_data](auto param) {
			if (callback != nullptr) {
				callback(device_ptr, static_cast<Parameter>(param), user_data);
			}
		});
		if (listener == nullptr) {
			set_sdk_last_error("Failed to subscribe length changed event: length listenr is null");
			return ERROR_EXCEPTION_WITH_MESSAGE;
		}
		*handle = reinterpret_cast<ListenerHandle>(new decltype(listener)(listener));
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

template <Neuro::ChannelInfo::Type ChannelType, typename DataArray>
auto setArrayDataListener(const Neuro::DeviceSharedPtr &device, Device *device_ptr, ChannelInfo info, void(*callback)(Device*, ChannelInfo, DataArray, void*), void* user_data) {
	auto listener = device->subscribeDataReceived<ChannelType>([device_ptr, callback, info, user_data](const auto &data) {
		if (callback != nullptr) {
			DataArray dataArray;
			using  TargetDataType = std::remove_pointer_t<decltype(dataArray.data_array)>;
			dataArray.samples_count = data.size();
			const auto memorySize = dataArray.samples_count * sizeof(TargetDataType);
			dataArray.data_array = reinterpret_cast<TargetDataType *>(malloc(memorySize));
			std::copy(data.begin(), data.end(), dataArray.data_array);
			callback(device_ptr, info, dataArray, user_data);
		}
	}, Neuro::ChannelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index));
	if (listener == nullptr) {
		throw std::runtime_error("Failed to subscribe length changed event: length listenr is null");
	}
	return reinterpret_cast<ListenerHandle>(new decltype(listener)(listener));
}

template <Neuro::ChannelInfo::Type ChannelType, typename DataArray>
auto setDataListener(const Neuro::DeviceSharedPtr &device, Device *device_ptr, ChannelInfo info, void(*callback)(Device*, ChannelInfo, DataArray, void*), void* user_data) {
	auto listener = device->subscribeDataReceived<ChannelType>([device_ptr, callback, info, user_data](const auto &data) {
		if (callback != nullptr) {
			DataArray dataArray;
			using  TargetDataType = std::remove_pointer_t<decltype(dataArray.data_array)>;
			dataArray.samples_count = 1;
			const auto memorySize = dataArray.samples_count * sizeof(TargetDataType);
			dataArray.data_array = reinterpret_cast<TargetDataType *>(malloc(memorySize));
			dataArray.data_array[0] = data;
			callback(device_ptr, info, dataArray, user_data);
		}
	}, Neuro::ChannelInfo(static_cast<Neuro::ChannelInfo::Type>(info.type), info.name, info.index));
	if (listener == nullptr) {
		throw std::runtime_error("Failed to subscribe length changed event: length listenr is null");
	}
	return reinterpret_cast<ListenerHandle>(new decltype(listener)(listener));
}

int device_subscribe_double_channel_data_received(Device *device_ptr, ChannelInfo channel_info,	void(*callback)(Device*, ChannelInfo, DoubleDataArray, void*), ListenerHandle *handle, void* user_data) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		switch (channel_info.type) {
		case ChannelTypeRespiration: {
			*handle = setArrayDataListener<Neuro::ChannelInfo::Type::Respiration, DoubleDataArray>(device, device_ptr, channel_info, callback, user_data);
			break;
		}
		case ChannelTypeResistance: {
			*handle = setArrayDataListener<Neuro::ChannelInfo::Type::Resistance, DoubleDataArray>(device, device_ptr, channel_info, callback, user_data);
			break;
		}
		default:
			throw std::runtime_error(std::string("Device doesn't have channel double channel ") + std::string(channel_info.name));
		}
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

int device_subscribe_int_channel_data_received(Device* device_ptr, ChannelInfo channel_info, void(*callback)(Device*, ChannelInfo, IntDataArray, void*), ListenerHandle *handle, void* user_data) {
	auto& device = *reinterpret_cast<Neuro::DeviceSharedPtr *>(device_ptr);
	try {
		switch (channel_info.type) {
		case ChannelTypeBattery: {
			*handle = setDataListener<Neuro::ChannelInfo::Type::Battery, IntDataArray>(device, device_ptr, channel_info, callback, user_data);
			break;
		}
		default:
			throw std::runtime_error(std::string("Device doesn't have channel double channel") + std::string(channel_info.name));
		}
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

void free_CommandArray(CommandArray command_array) {
	free(command_array.cmd_array);
}

void free_ParamInfoArray(ParamInfoArray param_info) {
	free(param_info.info_array);
}

void free_ChannelInfoArray(ChannelInfoArray channel_info) {
	free(channel_info.info_array);
}

void free_DoubleDataArray(DoubleDataArray data_array) {
	free(data_array.data_array);
}

void free_IntDataArray(IntDataArray data_array) {
	free(data_array.data_array);
}

