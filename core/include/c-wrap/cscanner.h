#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stddef.h>
#include "lib_export.h"
#include "clistener.h"

#ifdef __ANDROID__
#include <jni.h>
#endif

typedef struct _DevScanner DeviceScanner;
typedef struct _Device Device;

#ifdef __ANDROID__
SDK_SHARED DeviceScanner* create_device_scanner(jobject context);
#else
SDK_SHARED DeviceScanner* create_device_scanner();
#endif

SDK_SHARED void scanner_delete(DeviceScanner *);
SDK_SHARED int scanner_start_scan(DeviceScanner *, int timeout_ms);
SDK_SHARED int scanner_stop_scan(DeviceScanner *);
SDK_SHARED int scanner_is_scanning(DeviceScanner *, bool* out_is_scanning);
SDK_SHARED int scanner_set_device_found_callback(DeviceScanner *, void(*)(DeviceScanner *, Device *, void *), ListenerHandle *, void *user_data);
SDK_SHARED int scanner_set_scan_state_callback(DeviceScanner *, void(*)(DeviceScanner *, bool, void *), ListenerHandle *, void *user_data);
SDK_SHARED Device* scanner_get_device_by_address(DeviceScanner *, const char *address);

#endif // SCANNER_H