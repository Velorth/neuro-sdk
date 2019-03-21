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
using DeviceAddressType = NSUUID;
using BleEnumerator = MacOsBleEnumerator;
#elif __ANDROID__
#include "ble/android/android_ble_enumerator.h"
#include "ble/android/ble_device_jni.h"
namespace {
using DeviceAddressType = std::string;
using BleEnumerator = AndroidBleEnumerator;
}
#elif __linux__
    
#else
    static_assert(false);
#endif

#endif
