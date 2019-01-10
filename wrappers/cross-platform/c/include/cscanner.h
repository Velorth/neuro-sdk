#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stddef.h>
#include "lib_export.h"
#include "clistener.h"

typedef struct _DevScanner DeviceScanner;
typedef struct _Device Device;

SDK_SHARED DeviceScanner* create_device_scanner();
SDK_SHARED void scanner_delete(DeviceScanner *);
SDK_SHARED int scanner_start_scan(DeviceScanner *, int timeout_ms);
SDK_SHARED int scanner_stop_scan(DeviceScanner *);
SDK_SHARED int scanner_set_device_found_callback(DeviceScanner *, void(*)(Device *), ListenerHandle *);
SDK_SHARED int scanner_set_scan_state_callback(DeviceScanner *, void(*)(bool), ListenerHandle *);
SDK_SHARED Device* scanner_get_device_by_address(DeviceScanner *, const char *address);
SDK_SHARED int scanner_release_device(DeviceScanner *, const char *name, const char *address);

#endif // SCANNER_H
