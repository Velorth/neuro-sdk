#ifndef GATT_WRAPPER_H
#define GATT_WRAPPER_H

#include <windows.h>
#include <SetupAPI.h>
#include <devguid.h>
#include <memory>
#include <string>
#include <type_traits>

namespace Neuro {

std::string to_string(GUID);

BOOL DeviceInfoListUniversalDestroyer(HDEVINFO) noexcept;
HGLOBAL UniversalGlobalDeleter(HGLOBAL) noexcept;
BOOL UniversalCloseHandle(HANDLE) noexcept;

using DeviceInfoListPtr = std::unique_ptr<std::remove_pointer_t<HDEVINFO>, decltype(&DeviceInfoListUniversalDestroyer)>;
using DevInterfaceDetailData = std::unique_ptr<SP_DEVICE_INTERFACE_DETAIL_DATA, decltype(&UniversalGlobalDeleter)>;
using DeviceHandle = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype(&UniversalCloseHandle)>;

DeviceInfoListPtr make_device_list(GUID) noexcept;
DeviceHandle make_handle(const DevInterfaceDetailData &) noexcept;
DevInterfaceDetailData make_dev_interface_detail_data(DWORD size) noexcept;

}
#endif // GATT_WRAPPER_H
