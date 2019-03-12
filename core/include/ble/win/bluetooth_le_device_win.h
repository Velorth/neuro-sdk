#ifndef BLUETOOTH_LE_DEVICE_WIN_H
#define BLUETOOTH_LE_DEVICE_WIN_H

#include "ble/win/gatt_service_win.h"
#include "event_notifier.h"
#include <vector>
#include "ble/ble_device.h"

namespace Neuro {

struct DeviceInfo;

class BluetoothLEDeviceWin final {
public:
	explicit BluetoothLEDeviceWin(const DeviceInfo &);
	~BluetoothLEDeviceWin();

	std::vector<GattServiceWin> services() const;
	BleDeviceState connectionState() const;
	ListenerPtr<void, BleDeviceState> subscribeStateChanged(std::function<void(BleDeviceState)>);
private:
	winrt::Windows::Devices::Bluetooth::BluetoothLEDevice mDevice;
	Notifier<void, BleDeviceState> mStateNotifier;
};

}
#endif // BLUETOOTH_LE_DEVICE_WIN_H
