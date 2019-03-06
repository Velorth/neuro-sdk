#ifndef PLATFORM_TRAITS_H
#define PLATFORM_TRAITS_H

#ifdef _WIN32
#include "ble/win/windows_ble_enumerator.h"
#include "ble/win/ble_device_address.h"
namespace Neuro {
using DeviceAddressType = BleDeviceAddress;
using BleEnumerator = WindowsBleEnumerator;
}
#elif __APPLE__
    
    #if TARGET_IPHONE_SIMULATOR

    #elif TARGET_OS_IPHONE
        
    #elif TARGET_OS_MAC
        
    #else
        static_assert(false);
    #endif
#elif __ANDROID__
#include "ble/android/android_ble_enumerator.h"
namespace {
    using DeviceAddressType = unsigned long;
    using BleEnumerator = AndroidBleEnumerator;
}
#elif __linux__
    
#else
    static_assert(false);
#endif

#endif