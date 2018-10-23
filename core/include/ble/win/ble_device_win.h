#ifndef BLE_DEVICE_WIN
#define BLE_DEVICE_WIN

#include "ble/ble_device.h"
#include "winrt/Windows.Devices.Bluetooth.h"

namespace Neuro {

class BleDeviceWin : public BleDevice{
public:
	BleDeviceWin(unsigned long long address, const std::string &name);
    void connect() override;
    void disconnect() override;
    void close() override;
    bool sendCommand(const std::vector<Byte> &commandData) override;
    BleDeviceState getState() const override;
    std::string getName() const override;
    std::string getNetAddress() const override;

private:
    std::string mName;
	unsigned long long mAddress;
	winrt::Windows::Devices::Bluetooth::BluetoothLEDevice mBluetoothDevice{nullptr};
	bool mIsConnected{false};
    winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDeviceService mService{ nullptr };
    winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic mRxCharacteristic{ nullptr };
	winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic mTxCharacteristic{ nullptr };
    bool mHasStatusCharacteristic{false};
	winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattCharacteristic mStatusCharacteristic{ nullptr };
    winrt::Windows::Devices::Bluetooth::GenericAttributeProfile::GattDescriptor mCCCDDescriptor{ nullptr };

	void findServiceAndChars();
	void findAllCharacteristics();
	void setRxNotificationsAndFindTx();
	void findTxAndStausChars();
	void findStatusCharacteristic();
	void setStatusValueChangedNotifications();
	void onConnected();
	void onDisconnected();
};

}

#endif
