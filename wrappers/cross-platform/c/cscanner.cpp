extern "C"
{
#include "cscanner.h"
#include "sdk_error.h"
}
#include "device_scanner/scanner_factory.h"

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

int scanner_set_device_found_callback(DeviceScanner *scanner_ptr, void(*callback)(Device*)) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		scanner->subscribeDeviceFound([callback](auto device) {
			const auto device_raw_ptr = new Neuro::DeviceSharedPtr(std::move(device));
			if (callback != nullptr) {
				callback(reinterpret_cast<Device *>(device_raw_ptr));
			}
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

int scanner_set_scan_state_callback(DeviceScanner *scanner_ptr, void(*callback)(bool)) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		scanner->subscribeScanStateChanged([callback](bool is_scanning) {
			if (callback != nullptr) {
				callback(is_scanning);
			}
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

int scanner_release_device(DeviceScanner *scanner_ptr, const char *name, const char *address) {
	auto& scanner = *reinterpret_cast<std::unique_ptr<Neuro::DeviceScanner> *>(scanner_ptr);
	try {
		scanner->releaseDevice(name, address);
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
