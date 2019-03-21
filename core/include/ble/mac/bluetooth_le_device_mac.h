#ifndef BLUETOOTH_LE_DEVICE_MAC_H
#define BLUETOOTH_LE_DEVICE_MAC_H

#include "ble/mac/gatt_service_mac.h"
#include "event_notifier.h"
#include <vector>
#include "ble/ble_device.h"

namespace Neuro {

struct DeviceInfo;

class BluetoothLEDeviceMac final {
public:
	explicit BluetoothLEDeviceMac(const DeviceInfo &);
	~BluetoothLEDeviceMac();

	std::vector<GattServiceMac> services() const;
	BleDeviceState connectionState() const;
	ListenerPtr<void, BleDeviceState> subscribeStateChanged(std::function<void(BleDeviceState)>);
private:
	CBPeripheral* mDevice;
	Notifier<void, BleDeviceState> mStateNotifier;
};

}
#endif // BLUETOOTH_LE_DEVICE_MAC_H
