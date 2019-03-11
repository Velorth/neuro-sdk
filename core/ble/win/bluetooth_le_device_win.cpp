#include "ble/win/bluetooth_le_device_win.h"

namespace Neuro {

BluetoothLEDeviceWin::BluetoothLEDeviceWin(const DeviceInfo &device_info) {
	
}

std::vector<GattServiceWin> BluetoothLEDeviceWin::services() const {
	
}

BluetoothLEDeviceWin make_bluetooth_le_device(const DeviceInfo& device_info) {
	return BluetoothLEDeviceWin(device_info);
}

}
