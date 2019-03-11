#ifndef BLUETOOTH_LE_DEVICE_WIN_H
#define BLUETOOTH_LE_DEVICE_WIN_H


#include "device/device_info.h"
#include "ble/win/gatt_service_win.h"

namespace Neuro {

class BluetoothLEDeviceWin final {
public:
	explicit BluetoothLEDeviceWin(const DeviceInfo &);

	std::vector<GattServiceWin> services() const;
private:

};

BluetoothLEDeviceWin make_bluetooth_le_device(const DeviceInfo &device_info);

}
#endif // BLUETOOTH_LE_DEVICE_WIN_H
