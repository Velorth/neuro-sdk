#ifndef BLE_DEVICE_WRAPPER_H
#define BLE_DEVICE_WRAPPER_H

#include <memory>
#include "ble/ble_device.h"

namespace Neuro {

struct BleDeviceWrapperState;
struct DeviceInfo;

class BleDeviceWrapper final : public BleDevice {
public:
	explicit BleDeviceWrapper(const DeviceInfo &);
	~BleDeviceWrapper() override;
	void connect() override;
	void disconnect() override;
	void close() override;
	bool sendCommand(const std::vector<Byte>& commandData) override;
	BleDeviceState getState() const override;
	std::string getName() const override;
	std::string getNetAddress() const override;

private:
	std::unique_ptr<BleDeviceWrapperState> mState;
	ListenerPtr<void, const std::vector<Byte> &> mDataListener;
	ListenerPtr<void, const std::vector<Byte> &> mStatusListener;
	ListenerPtr<void, BleDeviceState> mConnectionListener;
};
}
#endif // BLE_DEVICE_WRAPPER_H
