#ifndef SCANNER_FACTORY_H
#define SCANNER_FACTORY_H

#include <memory>
#include "lib_export.h"
#include "device_scanner.h"
#ifdef __ANDROID__
#include <jni.h>
#endif

namespace Neuro {

#ifdef __ANDROID__
std::unique_ptr<DeviceScanner> createDeviceScanner(jobject context);
#else
SDK_SHARED std::unique_ptr<DeviceScanner> createDeviceScanner();
#endif

}
#endif // SCANNER_FACTORY_H
