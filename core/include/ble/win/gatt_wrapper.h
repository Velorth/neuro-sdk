#ifndef GATT_WRAPPER_H
#define GATT_WRAPPER_H

#include <windows.h>
#include <SetupAPI.h>
#include <regstr.h>
#include <bthdef.h>
#include <devguid.h>
#include <bluetoothleapis.h>
#include <cfgmgr32.h>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include "gsl/span"
#include "common_types.h"
#include <sstream>

namespace Neuro {

std::string to_string(GUID);

template <typename T, std::enable_if_t<std::is_integral<T>::value, T>* Deduce = nullptr>
std::string to_hex_string(T value){
    std::stringstream stream;
    stream << std::hex << value;
    return stream.str();
}

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

template <typename T, typename SizeType>
std::unique_ptr<T> alloc_smart_buffer(SizeType size){
    using SmartBufferPtr = std::unique_ptr<T>;
    auto rawByteBuffer = new Byte[size]();
    auto smartBuffer = SmartBufferPtr(reinterpret_cast<T *>(rawByteBuffer));
    return smartBuffer;
}

bool write_characteristic(const DeviceHandle &, BTH_LE_GATT_CHARACTERISTIC, const ByteBuffer &);
BTH_LE_GATT_CHARACTERISTIC get_device_characteristic(const DeviceHandle &, BTH_LE_GATT_SERVICE, std::string);
BTH_LE_GATT_SERVICE get_service(const DeviceHandle &, std::string);
BTH_LE_GATT_DESCRIPTOR get_descriptor(const DeviceHandle &, BTH_LE_GATT_CHARACTERISTIC, std::string);
BTH_LE_GATT_DESCRIPTOR_VALUE get_descriptor_value(const DeviceHandle &, BTH_LE_GATT_DESCRIPTOR);
bool set_descriptor_value(const DeviceHandle &, BTH_LE_GATT_DESCRIPTOR, BTH_LE_GATT_DESCRIPTOR_VALUE);
BLUETOOTH_GATT_EVENT_HANDLE subscribe_characteristic_value_changed(const DeviceHandle &, BTH_LE_GATT_CHARACTERISTIC, PFNBLUETOOTH_GATT_EVENT_CALLBACK, void *);

class gatt_timeout : public std::runtime_error{
public:
    explicit gatt_timeout(const std::string &message)
        : std::runtime_error(message){}

    explicit gatt_timeout(const char *message)
        : std::runtime_error(message){}
};

}
#endif // GATT_WRAPPER_H
