#ifndef PLATFORM_TRAITS_H
#define PLATFORM_TRAITS_H

#ifdef _WIN32
    
#elif __APPLE__
    #include "TargetConditionals.h"
    #include <impl/platforms/apple/apple_traits.h>
    #if TARGET_IPHONE_SIMULATOR
        using CurrentPlatform = bluetooth::_impl::IPhoneSimulator;
    #elif TARGET_OS_IPHONE
        
    #elif TARGET_OS_MAC
        
    #else
        static_assert(false);
    #endif
#elif __ANDROID__
    #include <impl/platforms/android/android_traits.h>
    using CurrentPlatform = bluetooth::_impl::Android;
#elif __linux__
    #include <impl/platforms/linux/linux_traits.h>
    using CurrentPlatform = bluetooth::_impl::Linux;
#else
    static_assert(false);
#endif

#endif