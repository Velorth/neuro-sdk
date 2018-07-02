#ifndef GATT_WRAPPER_H
#define GATT_WRAPPER_H

#include <windows.h>
#include <SetupAPI.h>
#include <regstr.h>
#include <bthdef.h>
#include <devguid.h>
#include <bluetoothleapis.h>
#include <memory>
#include <string>
#include <type_traits>
#include "gsl/span"
#include "common_types.h"

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

std::string to_string(GUID guid);
std::string to_narrow(const wchar_t *s, char def = '?', const std::locale& loc = std::locale());
std::string get_error_string(DWORD error_message_id);
GUID guid_from_string(const std::string &guid_string);

std::unique_ptr<wchar_t> generic_read_registry_property(const DeviceInfoListPtr &device_info_list, SP_DEVINFO_DATA dev_info_data, DWORD property);

template <typename T>
T convert_wchar_buffer(const std::unique_ptr<wchar_t> &);

template<>
inline std::string convert_wchar_buffer(const std::unique_ptr<wchar_t> &buffer){
    return to_narrow(buffer.get());
}

template<>
inline uint32_t convert_wchar_buffer(const std::unique_ptr<wchar_t> &buffer){
    return *reinterpret_cast<uint32_t*>(buffer.get());
}

template<DWORD Property>
auto convert_registry_property(const std::unique_ptr<wchar_t> &);

template<>
inline auto convert_registry_property<SPDRP_FRIENDLYNAME>(const std::unique_ptr<wchar_t> &property_buffer){
    return convert_wchar_buffer<std::string>(property_buffer);
}

template<>
inline auto convert_registry_property<SPDRP_HARDWAREID>(const std::unique_ptr<wchar_t> &property_buffer){
    return convert_wchar_buffer<std::string>(property_buffer);
}

template <DWORD Property>
auto read_registry_property(const DeviceInfoListPtr &device_info_list, const SP_DEVINFO_DATA &dev_info_data){
    auto propertyBuffer = generic_read_registry_property(device_info_list, dev_info_data, Property);
    return convert_registry_property<Property>(propertyBuffer);
}

bool write_characteristic(const DeviceHandle &, BTH_LE_GATT_CHARACTERISTIC, const ByteBuffer &);
BTH_LE_GATT_CHARACTERISTIC get_device_characteristic(const DeviceHandle &, BTH_LE_GATT_SERVICE, std::string);
BTH_LE_GATT_SERVICE get_service(const DeviceHandle &, std::string);

}
#endif // GATT_WRAPPER_H
