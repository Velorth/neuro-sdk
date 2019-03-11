#include "ble/ble_device_wrapper.h"
#include "ble/platform_traits_impl.h"

namespace Neuro {

struct ClosedImpl;
struct DisconnectedImpl;
struct ConnectedImpl;

struct BleDeviceWrapperState {
	virtual ~BleDeviceWrapperState() = default;
	virtual std::unique_ptr<BleDeviceWrapperState> getClosedImpl() = 0;
	virtual std::unique_ptr<BleDeviceWrapperState> getDisconnectedImpl() = 0;
	virtual std::unique_ptr<BleDeviceWrapperState> getConnectedImpl() = 0;

	virtual std::string getName() = 0;
	virtual std::string getAddress() = 0;
	virtual BleDeviceState getState() = 0;
	virtual bool sendCommand(const std::vector<Byte>& commandData) = 0;

	virtual ListenerPtr<void, const std::vector<Byte> &>
	subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) = 0;

	virtual ListenerPtr<void, const std::vector<Byte> &> 
	subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) = 0;
};

static std::unique_ptr<BleDeviceWrapperState> make_closed_impl();
static std::unique_ptr<BleDeviceWrapperState> make_disconnected_impl(const DeviceInfo &info, const DeviceGattInfo &gatt_info);
static std::unique_ptr<BleDeviceWrapperState> make_connected_impl(const DeviceInfo &info, const DeviceGattInfo &gatt_info);

struct ClosedImpl final : public BleDeviceWrapperState {
	std::unique_ptr<BleDeviceWrapperState> getClosedImpl() override {
		return make_closed_impl();
	}

	std::unique_ptr<BleDeviceWrapperState> getDisconnectedImpl() override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	std::unique_ptr<BleDeviceWrapperState> getConnectedImpl() override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	std::string getName() override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	std::string getAddress() override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	BleDeviceState getState() override {
		return BleDeviceState::Error;
	}

	bool sendCommand(const std::vector<Byte>& commandData) override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}
};

struct DisconnectedImpl final : public BleDeviceWrapperState {
	DisconnectedImpl(const DeviceInfo &info, const DeviceGattInfo &gatt_info) :
		mInfo(info),
		mGattInfo(gatt_info){}

	std::unique_ptr<BleDeviceWrapperState> getClosedImpl() override {
		return make_closed_impl();
	}

	std::unique_ptr<BleDeviceWrapperState> getDisconnectedImpl() override {
		return make_disconnected_impl(mInfo, mGattInfo);
	}

	std::unique_ptr<BleDeviceWrapperState> getConnectedImpl() override {
		return make_connected_impl(mInfo, mGattInfo);
	}

	std::string getName() override {
		return mInfo.Name;
	}

	std::string getAddress() override {
		using std::to_string;
		return to_string(mInfo.Address);
	}

	BleDeviceState getState() override {
		return BleDeviceState::Disconnected;
	}

	bool sendCommand(const std::vector<Byte>& commandData) override {
		throw std::runtime_error("Unable to send command. Device is disconnected");
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) {
		return nullptr;
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) {
		return nullptr;
	}

private:
	const DeviceInfo mInfo;
	const DeviceGattInfo &mGattInfo;
};

GattCharacteristic find_tx_characteristic(BluetoothLEDevice device, const DeviceGattInfo &gatt_info) {
	auto services = device.services();
	auto guid = guid_from_string(gatt_info.deviceServiceUUID());
	const auto serviceIterator = std::find_if(services.begin(), services.end(), [](const auto &service) { return service.uuid() == guid; });
	if (serviceIterator == services.end()) {
		throw std::runtime_error("Main service not found");
	}


}

struct ConnectedImpl final : public BleDeviceWrapperState {
	ConnectedImpl(const DeviceInfo &info, const DeviceGattInfo &gatt_info) :
		mInfo(info),
		mGattInfo(gatt_info),
		mDevice(mInfo),
		mTxCharacteristic(find_tx_characteristic(mDevice, mGattInfo)){}

	std::unique_ptr<BleDeviceWrapperState> getClosedImpl() override {
		return make_closed_impl();
	}

	std::unique_ptr<BleDeviceWrapperState> getDisconnectedImpl() override {
		return make_disconnected_impl(mInfo, mGattInfo);
	}

	std::unique_ptr<BleDeviceWrapperState> getConnectedImpl() override {
		return make_connected_impl(mInfo, mGattInfo);
	}

	std::string getName() override {
		return mInfo.Name;
	}

	std::string getAddress() override {
		using std::to_string;
		return to_string(mInfo.Address);
	}

	BleDeviceState getState() override {
		return BleDeviceState::Connected;
	}

	bool sendCommand(const std::vector<Byte>& commandData) override {
		mTxCharacteristic.write(commandData);
		return true;
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) {
		return mDataReceivedCallback.addListener(callback);
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) {
		return mStatusReceivedCallback.addListener(callback);
	}

private:
	const DeviceInfo mInfo;
	const DeviceGattInfo &mGattInfo;
	BluetoothLEDevice mDevice;
	GattCharacteristic mTxCharacteristic;
	Notifier<void, const std::vector<Byte> &> mDataReceivedCallback;
	Notifier<void, const std::vector<Byte> &> mStatusReceivedCallback;
};

static std::unique_ptr<BleDeviceWrapperState> make_closed_impl() {
	return std::make_unique<ClosedImpl>();
}

static std::unique_ptr<BleDeviceWrapperState> make_disconnected_impl(const DeviceInfo &device_info, const DeviceGattInfo &gatt_info) {
	return std::make_unique<DisconnectedImpl>(device_info, gatt_info);
}

static std::unique_ptr<BleDeviceWrapperState> make_connected_impl(const DeviceInfo &device_info, const DeviceGattInfo &gatt_info) {
	return std::make_unique<ConnectedImpl>(device_info, gatt_info);
}

static std::unique_ptr<BleDeviceWrapperState> make_init_state(const DeviceInfo &device_info, const DeviceGattInfo &gatt_info) {
	return std::make_unique<DisconnectedImpl>(device_info, gatt_info);
}

BleDeviceWrapper::BleDeviceWrapper(const DeviceInfo &device_info):
	BleDevice(BleDeviceInfo::fromDeviceName(device_info.Name)),
	mState(make_init_state(device_info, *getGattInfo()->getGattInfo())),
	mDataListener(mState->subscribeDataReceived([=](const auto &data) { notifyDataReceived(data); })),
	mStatusListener(mState->subscribeStatusReceived([=](const auto &data) { notifyStatusReceived(data); })) {
}

BleDeviceWrapper::~BleDeviceWrapper() = default;

void BleDeviceWrapper::connect() {
	if (mState->getState() != BleDeviceState::Connected) {
		mState = mState->getConnectedImpl();
		mDataListener = mState->subscribeDataReceived([=](const auto &data) { notifyDataReceived(data); });
		mStatusListener = mState->subscribeStatusReceived([=](const auto &data) { notifyStatusReceived(data); });
		notifyStateChanged(BleDeviceState::Connected, BleDeviceError::NoError);
	}
}

void BleDeviceWrapper::disconnect() {
	if (mState->getState() != BleDeviceState::Disconnected) {
		mState = mState->getDisconnectedImpl();
		notifyStateChanged(BleDeviceState::Disconnected, BleDeviceError::NoError);
	}
}

void BleDeviceWrapper::close() {
	if (mState->getState() != BleDeviceState::Error) {
		mState = mState->getClosedImpl();
		notifyStateChanged(BleDeviceState::Error, BleDeviceError::NoError);
	}
}

bool BleDeviceWrapper::sendCommand(const std::vector<Byte>& commandData) {
	return mState->sendCommand(commandData);
}

BleDeviceState BleDeviceWrapper::getState() const {
	return mState->getState();
}

std::string BleDeviceWrapper::getName() const {
	return mState->getName();
}

std::string BleDeviceWrapper::getNetAddress() const {
	return mState->getAddress();
}

}
