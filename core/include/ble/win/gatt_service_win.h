#ifndef GATT_SERVICE_WIN_H
#define GATT_SERVICE_WIN_H

#include <vector>
#include "ble/win/gatt_characteristic_win.h"

namespace Neuro {

class GattServiceWin final {
public:	
	GUID uuid() const;
	std::vector<GattCharacteristicWin> characteristics() const;
private:
	winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService mService;
};

}

#endif // GATT_SERVICE_WIN_H
