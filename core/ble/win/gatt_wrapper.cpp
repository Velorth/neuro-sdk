#include <algorithm>
#include <sstream>
#include "ble/win/gatt_wrapper.h"
#include "logger.h"

namespace Neuro{

BOOL DeviceInfoListUniversalDestroyer(HDEVINFO dev_info) noexcept {
    if (dev_info == nullptr){
        return true;
    }
    return SetupDiDestroyDeviceInfoList(dev_info);
}

HGLOBAL UniversalGlobalDeleter(HGLOBAL global) noexcept {
    if (global == nullptr){
        return nullptr;
    }
    return GlobalFree(global);
}

BOOL UniversalCloseHandle(HANDLE handle) noexcept {
    if (handle == nullptr){
        return true;
    }
    return CloseHandle(handle);
}

std::string to_string(GUID guid){
    char guid_cstr[39];
    snprintf(guid_cstr, sizeof(guid_cstr),
             "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
             guid.Data1, guid.Data2, guid.Data3,
             guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

    return std::string(guid_cstr);
}

std::string to_narrow(const wchar_t *s, char def, const std::locale& loc){
    std::ostringstream stm;
    while (*s != L'\0') {
        stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, def);
    }
    return stm.str();
}

std::string get_error_string(DWORD error_message_id){
    if(error_message_id == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error_message_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);

    return message;
}

GUID guid_from_string(const std::string &guid_string){
    GUID guid;
    sscanf(guid_string.c_str(),
           "%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
           &guid.Data1, &guid.Data2, &guid.Data3,
           &guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
            &guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7] );
    return guid;
}

DeviceInfoListPtr make_device_list(GUID serviceGuid) noexcept {
    auto listPtr = SetupDiGetClassDevs(&serviceGuid, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    return DeviceInfoListPtr(listPtr, &DeviceInfoListUniversalDestroyer);
}

DeviceHandle make_handle(const DevInterfaceDetailData &interface_detail_data) noexcept {
    auto deviceHandle = CreateFile(
                interface_detail_data->DevicePath,
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_EXISTING,
                0,
                NULL);
    return DeviceHandle(deviceHandle, &UniversalCloseHandle);
}

DevInterfaceDetailData make_dev_interface_detail_data(DWORD size) noexcept {
    auto detailedDataPtr = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(GlobalAlloc(GPTR, size));
    return DevInterfaceDetailData(detailedDataPtr, &UniversalGlobalDeleter);
}

std::unique_ptr<wchar_t> generic_read_registry_property(const DeviceInfoListPtr &device_info_list, SP_DEVINFO_DATA dev_info_data, DWORD property){
    std::unique_ptr<wchar_t> propertyBuffer;
    DWORD propertyBufferSize = 0;
    while (!SetupDiGetDeviceRegistryProperty(
               device_info_list.get(),
               &dev_info_data,
               property,
               NULL,
               (PBYTE)propertyBuffer.get(),
               propertyBufferSize,
               &propertyBufferSize)){
        auto lastError = GetLastError();
        if (lastError == ERROR_INSUFFICIENT_BUFFER){
            propertyBuffer = std::unique_ptr<wchar_t>(new wchar_t[propertyBufferSize * 2]);
        }else{
            auto excMsg = std::string("Unable read property: Error code ") + std::to_string(lastError) + std::string(" - ") + get_error_string(lastError);
            throw std::runtime_error(excMsg.c_str());
        }
    }
    return propertyBuffer;
}

bool write_characteristic(const DeviceHandle &device, BTH_LE_GATT_CHARACTERISTIC characteristic, const ByteBuffer &data){
    using CharValuePtr = std::unique_ptr<BTH_LE_GATT_CHARACTERISTIC_VALUE>;
    auto requiredLength = data.size() + offsetof(BTH_LE_GATT_CHARACTERISTIC_VALUE, Data);
    auto charValuePtr = CharValuePtr(reinterpret_cast<BTH_LE_GATT_CHARACTERISTIC_VALUE*>(new Byte[requiredLength]));
    charValuePtr->DataSize = data.size();
    std::copy(data.begin(), data.end(), charValuePtr->Data);
    auto result = BluetoothGATTSetCharacteristicValue(device.get(),
                                               &characteristic,
                                               charValuePtr.get(),
                                               NULL,
                                               BLUETOOTH_GATT_FLAG_WRITE_WITHOUT_RESPONSE) == S_OK;
    return result;
}

BTH_LE_GATT_CHARACTERISTIC get_device_characteristic(const DeviceHandle &device, BTH_LE_GATT_SERVICE service, std::string guid_string) {
    auto charBufferSize = static_cast<USHORT>(0);
    if (BluetoothGATTGetCharacteristics(
                device.get(),
                &service,
                0,
                NULL,
                &charBufferSize,
                BLUETOOTH_GATT_FLAG_NONE) != HRESULT_FROM_WIN32(ERROR_MORE_DATA)) {
        throw std::runtime_error("Unable to list device characteristics");
    }
    if (charBufferSize == 0) {
        throw std::runtime_error("Service does not have characteristics");
    }

    using CharBufferPtr = std::unique_ptr<BTH_LE_GATT_CHARACTERISTIC>;
    auto charByteBuffer = new Byte[charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC)]();
    auto charsBuffer = CharBufferPtr(reinterpret_cast<PBTH_LE_GATT_CHARACTERISTIC>(charByteBuffer));

    auto charsCount = static_cast<USHORT>(0);
    if (BluetoothGATTGetCharacteristics(
            device.get(),
            &service,
            charBufferSize,
            charsBuffer.get(),
            &charsCount,
            BLUETOOTH_GATT_FLAG_NONE) != S_OK ) {
        throw std::runtime_error("Unable to read characteristics buffer");
    }
    if (charsCount != charBufferSize) {
        throw std::runtime_error("Characteristics buffer size mismatch");
    }

    auto charGuid = guid_from_string(guid_string);
    for (decltype(charBufferSize) i = 0; i < charBufferSize; ++i){
        auto discoveredChar = charsBuffer.get()[i];
        if (charGuid == discoveredChar.CharacteristicUuid.Value.LongUuid){
            return discoveredChar;
        }
    }
    throw std::runtime_error("Characteristic not found");
}

BTH_LE_GATT_SERVICE get_service(const DeviceHandle &device, std::string service_guid_str){
    auto serviceBufferSize = USHORT{0};
    if (BluetoothGATTGetServices(
                device.get(),
                0,
                NULL,
                &serviceBufferSize,
                BLUETOOTH_GATT_FLAG_NONE) != HRESULT_FROM_WIN32(ERROR_MORE_DATA)) {
        throw std::runtime_error("Unable to list device services");
    }
    if (serviceBufferSize == 0) {
        throw std::runtime_error("Device does not have services");
    }

    using ServiceBufferPtr = std::unique_ptr<BTH_LE_GATT_SERVICE>;
    auto serviceByteBuffer = new Byte[serviceBufferSize * sizeof(BTH_LE_GATT_SERVICE)]();
    auto serviceBuffer = ServiceBufferPtr(reinterpret_cast<PBTH_LE_GATT_SERVICE>(serviceByteBuffer));

    auto servicesCount = USHORT{0};
    if (BluetoothGATTGetServices(
                device.get(),
                serviceBufferSize,
                serviceBuffer.get(),
                &servicesCount,
                BLUETOOTH_GATT_FLAG_NONE) != S_OK) {
        throw std::runtime_error("Unable to read services buffer");
    }
    if (servicesCount != serviceBufferSize) {
        throw std::runtime_error("Services buffer size mismatch");
    }

    auto serviceGuid = guid_from_string(service_guid_str);
    for (auto i = decltype(serviceBufferSize){0}; i < serviceBufferSize; ++i){
        auto discoveredService = serviceBuffer.get()[i];
        if (serviceGuid == discoveredService.ServiceUuid.Value.LongUuid){
            return discoveredService;
        }
    }
    throw std::runtime_error("Service not found");
}

}
