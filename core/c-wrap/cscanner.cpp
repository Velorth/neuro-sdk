extern "C"
{
#include "cscanner.h"
#include "sdk_error.h"
}
#include "device_scanner/scanner_factory.h"

#ifdef __ANDROID__
DeviceScanner* create_device_scanner(jobject context) {
	try {
		auto scanner = Neuro::createDeviceScanner(context);
		const auto scanner_ptr = new decltype(scanner)(std::move(scanner));
		return reinterpret_cast<DeviceScanner *>(scanner_ptr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
}
#else
DeviceScanner* create_device_scanner() {
	try {
		auto scanner = Neuro::createDeviceScanner();
		const auto scanner_ptr = new decltype(scanner)(std::move(scanner));
		return reinterpret_cast<DeviceScanner *>(scanner_ptr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
}
#endif

void scanner_delete(DeviceScanner *scanner_ptr) {
	const auto scanner_unique = reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	delete scanner_unique;
}

int scanner_start_scan(DeviceScanner *scanner_ptr, int timeout_ms) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		scanner->startScan(timeout_ms);
		return SDK_NO_ERROR;
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return ERROR_EXCEPTION_WITH_MESSAGE;
	}
	catch(...) {
		return ERROR_UNHANDLED_EXCEPTION;
	}
}

int scanner_stop_scan(DeviceScanner *scanner_ptr) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		scanner->stopScan();
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

int scanner_is_scanning(DeviceScanner *scanner_ptr, bool *out_is_scanning) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		*out_is_scanning = scanner->isScanning();
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

int scanner_set_device_found_callback(DeviceScanner *scanner_ptr, void(*callback)(DeviceScanner *, Device*, void *), ListenerHandle* handle, void *user_data) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		auto listener = scanner->subscribeDeviceFound([scanner_ptr, callback, user_data](auto device) {
			const auto device_raw_ptr = new Neuro::DeviceSharedPtr(std::move(device));
			if (callback != nullptr) {
				callback(scanner_ptr, reinterpret_cast<Device *>(device_raw_ptr), user_data);
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

int scanner_set_scan_state_callback(DeviceScanner *scanner_ptr, void(*callback)(DeviceScanner *, bool, void*), ListenerHandle *handle, void *user_data) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		auto listener = scanner->subscribeScanStateChanged([scanner_ptr, callback, user_data](bool is_scanning) {
			if (callback != nullptr) {
				callback(scanner_ptr, is_scanning, user_data);
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

Device* scanner_get_device_by_address(DeviceScanner *scanner_ptr, const char *address) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		auto device = scanner->findDeviceByAddress(address);
		if (device == nullptr) {
			return nullptr;
		}
		const auto device_raw_ptr = new decltype(device)(std::move(device));
		return reinterpret_cast<Device *>(device_raw_ptr);
	}
	catch (std::exception &e) {
		set_sdk_last_error(e.what());
		return nullptr;
	}
	catch (...) {
		return nullptr;
	}
}

