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

}
