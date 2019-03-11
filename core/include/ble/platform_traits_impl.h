#ifndef PLATFORM_TRAITS_IMPL_H
#define PLATFORM_TRAITS_IMPL_H

#include "platform_traits.h"

#ifdef _WIN32
#include <guiddef.h>
#include <winrt/Windows.Foundation.h>
#include "win/bluetooth_le_device_win.h"
#include "win/gatt_characteristic_win.h"
#include "win/gatt_service_win.h"

namespace Neuro {
using UUIDType = _GUID;
using BleTimeType = winrt::Windows::Foundation::DateTime;
using BleTimeDuration = BleTimeType::duration;
using BluetoothLEDevice = BluetoothLEDeviceWin;
using GattService = GattServiceWin;
using GattCharacteristic = GattCharacteristicWin;

static UUIDType guid_from_string(const std::string &guid_string) {
	GUID guid;
	sscanf(guid_string.c_str(),
		"%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
		&guid.Data1, &guid.Data2, &guid.Data3,
		&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
		&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);
	return guid;
}

}
#elif __APPLE__
    
    #if TARGET_IPHONE_SIMULATOR

    #elif TARGET_OS_IPHONE
        
    #elif TARGET_OS_MAC
        
    #else
        static_assert(false);
    #endif
#elif __ANDROID__
namespace Neuro {
using UUIDType = _GUID;
using BleTimeType = winrt::Windows::Foundation::DateTime;
using BleTimeDuration = BleTimeType::duration;

static UUIDType guid_from_string(const std::string &guid_string) {
	return guid_string;
}

}
#elif __linux__
    
#else
    static_assert(false);
#endif

#endif