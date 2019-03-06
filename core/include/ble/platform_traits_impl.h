#ifndef PLATFORM_TRAITS_IMPL_H
#define PLATFORM_TRAITS_IMPL_H

#include "platform_traits.h"

#ifdef _WIN32
#include <guiddef.h>
#include <winrt/Windows.Foundation.h>
namespace Neuro {
using UUIDType = _GUID;
using BleTimeType = winrt::Windows::Foundation::DateTime;
using BleTimeDuration = BleTimeType::duration;
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
}
#elif __linux__
    
#else
    static_assert(false);
#endif

#endif