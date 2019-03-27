#ifndef PLATFORM_TRAITS_H
#define PLATFORM_TRAITS_H

#ifdef _WIN32
#include "ble/win/windows_ble_enumerator.h"
#include "ble/win/ble_device_address.h"
#include "ble/win/ble_device_wrapper.h"
namespace Neuro {
using DeviceAddressType = BleDeviceAddress;
using BleEnumerator = WindowsBleEnumerator;
using PlatformBleDeviceWrapper = BleDeviceWrapper;
}
#elif __APPLE__
#include "ble/mac/macos_ble_enumerator.h"
#include "ble/ios/ble_device_objc.h"
namespace Neuro {
using DeviceAddressType = std::string;
using BleEnumerator = MacOsBleEnumerator;
using PlatformBleDeviceWrapper = NCBleDevice;
}
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
