extern "C"
{
#include "cparams.h"
#include "sdk_error.h"
}

#include "device/device.h"
#include "device/param_values.h"

extern std::string sdk_last_error;

ret_code device_read_Name(Device *device_ptr, char* out_name) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::Name>();
		strcpy(out_name, value.c_str());
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

ret_code device_read_State(Device *device_ptr, DeviceState* out_state) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::State>();
		*out_state = static_cast<DeviceState>(value);
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

ret_code device_read_Address(Device *device_ptr, char* out_address) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::Address>();
		strcpy(out_address, value.c_str());
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

ret_code device_read_SerialNumber(Device *device_ptr, char* out_serial) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::SerialNumber>();
		strcpy(out_serial, value.c_str());
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

ret_code device_read_HardwareFilterState(Device *device_ptr, bool* out_is_enabled) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		const auto value = device->readParam<Neuro::Parameter::HardwareFilterState>();
		*out_is_enabled = value;
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

ret_code device_read_FirmwareMode(Device *device_ptr, FirmwareMode* out_mode) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::FirmwareMode>();
		*out_mode = static_cast<FirmwareMode>(value);
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

ret_code device_read_SamplingFrequency(Device *device_ptr, SamplingFrequency* out_freq) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::SamplingFrequency>();
		*out_freq = static_cast<SamplingFrequency>(value);
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

ret_code device_read_Gain(Device *device_ptr, Gain* out_gain) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::Gain>();
		*out_gain = static_cast<Gain>(value);
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

ret_code device_read_Offset(Device *device_ptr, unsigned char* out_offset) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		const auto value = device->readParam<Neuro::Parameter::Offset>();
		*out_offset = value;
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

ret_code device_read_ExternalSwitchState(Device *device_ptr, ExternalSwitchInput* out_ext_switch) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::ExternalSwitchState>();
		*out_ext_switch = static_cast<ExternalSwitchInput>(value);
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

ret_code device_read_ADCInputState(Device *device_ptr, ADCInput* out_adc_input) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::ADCInputState>();
		*out_adc_input = static_cast<ADCInput>(value);
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

ret_code device_read_AccelerometerSens(Device *device_ptr, AccelerometerSensitivity* out_accel_sens) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::AccelerometerSens>();
		*out_accel_sens = static_cast<AccelerometerSensitivity>(value);
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

ret_code device_read_GyroscopeSens(Device *device_ptr, GyroscopeSensitivity* out_guro_sens) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		auto value = device->readParam<Neuro::Parameter::GyroscopeSens>();
		*out_guro_sens = static_cast<GyroscopeSensitivity>(value);
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

ret_code device_read_StimulatorState(Device *device_ptr, bool* out_is_enabled) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		const auto value = device->readParam<Neuro::Parameter::StimulatorState>();
		*out_is_enabled = value;
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

ret_code device_read_MotionAssistantState(Device *device_ptr, bool* out_is_enabled) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		const auto value = device->readParam<Neuro::Parameter::MotionAssistantState>();
		*out_is_enabled = value;
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

ret_code device_read_StimulatorParamPack(Device *device_ptr, StimulationParams* out_stimul_params) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		const auto value = device->readParam<Neuro::Parameter::StimulatorParamPack>();
		out_stimul_params->current = value.current;
		out_stimul_params->frequency = value.frequency;
		out_stimul_params->pulse_duration = value.pulse_duration;
		out_stimul_params->stimulus_duration = value.stimulus_duration;
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

ret_code device_read_MotionAssistantParamPack(Device *device_ptr, MotionAssistantParams* out_ma_params) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		const auto value = device->readParam<Neuro::Parameter::MotionAssistantParamPack>();
		out_ma_params->gyroStart = value.gyroStart;
		out_ma_params->gyroStop = value.gyroStop;
		out_ma_params->limb = static_cast<MotionAssistantLimb>(value.limb);
		out_ma_params->maxDuration = value.maxDuration;
		out_ma_params->minPause = value.minPause;
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



ret_code device_set_Name(Device *device_ptr, const char* name) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::Name>(name);
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

ret_code device_set_State(Device *device_ptr, DeviceState state) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::State>(static_cast<Neuro::DeviceState>(state));
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

ret_code device_set_Address(Device *device_ptr, const char* address) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::Address>(address);
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

ret_code device_set_SerialNumber(Device *device_ptr, const char* serial) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::SerialNumber>(serial);
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

ret_code device_set_HardwareFilterState(Device *device_ptr, bool is_enabled) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::HardwareFilterState>(is_enabled);
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

ret_code device_set_FirmwareMode(Device *device_ptr, FirmwareMode mode) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::FirmwareMode>(static_cast<Neuro::FirmwareMode>(mode));
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

ret_code device_set_SamplingFrequency(Device *device_ptr, SamplingFrequency freq) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::SamplingFrequency>(static_cast<Neuro::SamplingFrequency>(freq));
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

ret_code device_set_Gain(Device *device_ptr, Gain gain) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::Gain>(static_cast<Neuro::Gain>(gain));
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

ret_code device_set_Offset(Device *device_ptr, unsigned char offset) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::Offset>(offset);
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

ret_code device_set_ExternalSwitchState(Device *device_ptr, ExternalSwitchInput ext_switch) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::ExternalSwitchState>(static_cast<Neuro::ExternalSwitchInput>(ext_switch));
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

ret_code device_set_ADCInputState(Device *device_ptr, ADCInput adc_input) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::ADCInputState>(static_cast<Neuro::ADCInput>(adc_input));
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

ret_code device_set_AccelerometerSens(Device *device_ptr, AccelerometerSensitivity accel_sens) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::AccelerometerSens>(static_cast<Neuro::AccelerometerSensitivity>(accel_sens));
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

ret_code device_set_GyroscopeSens(Device *device_ptr, GyroscopeSensitivity gyro_sens) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::GyroscopeSens>(static_cast<Neuro::GyroscopeSensitivity>(gyro_sens));
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

ret_code device_set_StimulatorState(Device *device_ptr, bool is_enabled) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::StimulatorState>(is_enabled);
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

ret_code device_set_MotionAssistantState(Device *device_ptr, bool is_enabled) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		device->setParam<Neuro::Parameter::MotionAssistantState>(is_enabled);
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

ret_code device_set_StimulatorParamPack(Device *device_ptr, StimulationParams stimul_params) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		Neuro::StimulationParams params{};
		params.current = stimul_params.current;
		params.frequency = stimul_params.frequency;
		params.pulse_duration = stimul_params.pulse_duration;
		params.stimulus_duration = stimul_params.stimulus_duration;
		device->setParam<Neuro::Parameter::StimulatorParamPack>(params);
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

ret_code device_set_MotionAssistantParamPack(Device *device_ptr, MotionAssistantParams ma_params) {
	auto& device = *reinterpret_cast<std::unique_ptr<Neuro::Device> *>(device_ptr);
	try {
		Neuro::MotionAssistantParams params{};
		params.gyroStart = ma_params.gyroStart;
		params.gyroStop = ma_params.gyroStop;
		params.limb = static_cast<Neuro::MotionAssistantLimb>(ma_params.limb);
		params.maxDuration = ma_params.maxDuration;
		params.minPause = ma_params.minPause;
		device->setParam<Neuro::Parameter::MotionAssistantParamPack>(params);
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
