#include "ble/win/ble_device_wrapper.h"
#include "ble/platform_traits_impl.h"
#include "device/device_info.h"
#include "logger.h"

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

	virtual ListenerPtr<void, BleDeviceState>
	subscribeConnectionStateChanged(const std::function<void(BleDeviceState)> &callback) = 0;
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

	ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) override {
		throw std::runtime_error("Cannot perform operation on device. Device is closed");
	}

	ListenerPtr<void, BleDeviceState> subscribeConnectionStateChanged(const std::function<void(BleDeviceState)> &callback) override {
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

	ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) override {
		return nullptr;
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) override {
		return nullptr;
	}

	ListenerPtr<void, BleDeviceState> subscribeConnectionStateChanged(const std::function<void(BleDeviceState)> &callback) override {
		return nullptr;
	}

private:
	const DeviceInfo mInfo;
	const DeviceGattInfo &mGattInfo;
};

GattService find_service(const BluetoothLEDevice &device, const std::string &service_guid) {
	auto services = device.services();
	auto serviceGuid = guid_from_string(service_guid);
	const auto serviceIterator = std::find_if(services.begin(), services.end(), [&serviceGuid](const auto &service) { return service.uuid() == serviceGuid; });
	if (serviceIterator == services.end()) {
		throw std::runtime_error("Main service not found");
	}

	return std::move(*serviceIterator);
}

GattCharacteristic find_characteristic(const GattService &service, const std::string &char_guid) {
	auto txGuid = guid_from_string(char_guid);
	auto characteristics = service.characteristics();
	const auto characteristic = std::find_if(characteristics.begin(), characteristics.end(), [&txGuid](const auto &characteristic) { return characteristic.uuid() == txGuid; });
	if (characteristic == characteristics.end()) {
		throw std::runtime_error("Requested characteristic not found");
	}
	return std::move(*characteristic);
}

struct ConnectedImpl final : public BleDeviceWrapperState {

	template <typename... DeviceArgs>
	ConnectedImpl(const DeviceInfo &info, const DeviceGattInfo &gatt_info, DeviceArgs... args) :
		mInfo(info),
		mGattInfo(gatt_info),
		mDevice(mInfo, args...),
		mService(find_service(mDevice, mGattInfo.deviceServiceUUID())),
		mTxCharacteristic(find_characteristic(mService, mGattInfo.txCharacteristicUUID())),
		mRxCharacteristic(find_characteristic(mService, mGattInfo.rxCharacteristicUUID())){
		subscribeDataReceived();
		subscribeStatusReceived();
	}

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
		return mDevice.connectionState();
	}

	bool sendCommand(const std::vector<Byte>& commandData) override {
		try {
			mTxCharacteristic.write(commandData);
			return true;
		}
		catch (std::exception &e) {
			LOG_ERROR_V("Failed write tx characteristic: %s", e.what());
			return false;
		}
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeDataReceived(const std::function<void(const std::vector<Byte> &)> &callback) override {
		return mDataReceivedCallback.addListener(callback);
	}

	ListenerPtr<void, const std::vector<Byte> &> subscribeStatusReceived(const std::function<void(const std::vector<Byte> &)> &callback) override {
		return mStatusReceivedCallback.addListener(callback);
	}

	ListenerPtr<void, BleDeviceState> subscribeConnectionStateChanged(const std::function<void(BleDeviceState)>& callback) override {
		return mDevice.subscribeStateChanged(callback);
	}

private:
	static constexpr const char *class_name = "BleDeviceWrapper";
	const DeviceInfo mInfo;
	const DeviceGattInfo &mGattInfo;
	BluetoothLEDevice mDevice;
	GattService mService;
	GattCharacteristic mTxCharacteristic;
	GattCharacteristic mRxCharacteristic;
	std::unique_ptr<GattCharacteristic> mStatusCharacteristic;
	ListenerPtr<void, const std::vector<Byte> &> mDataListenHandle;
	ListenerPtr<void, const std::vector<Byte> &> mStatusListenHandle;
	Notifier<void, const std::vector<Byte> &> mDataReceivedCallback;
	Notifier<void, const std::vector<Byte> &> mStatusReceivedCallback;

	void subscribeDataReceived() {
		mDataListenHandle = mRxCharacteristic.subscribeCharacteristicChanged([=](const auto &data) { mDataReceivedCallback.notifyAll(data); });
	}

	void subscribeStatusReceived() {
		if (mGattInfo.statusCharacteristicUUID().empty())
			return;

		mStatusCharacteristic = std::make_unique<GattCharacteristic>(find_characteristic(mService, mGattInfo.statusCharacteristicUUID()));
		mStatusListenHandle = mStatusCharacteristic->subscribeCharacteristicChanged([=](const auto &data) { mStatusReceivedCallback.notifyAll(data); });
	}
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
	mStatusListener(mState->subscribeStatusReceived([=](const auto &data) { notifyStatusReceived(data); })),
	mConnectionListener(mState->subscribeConnectionStateChanged([=](const auto &state) { notifyStateChanged(state, BleDeviceError::NoError); })) {
}

BleDeviceWrapper::~BleDeviceWrapper() = default;

void BleDeviceWrapper::connect() {
	if (mState->getState() != BleDeviceState::Connected) {
		try {
			mState = mState->getConnectedImpl();
			mDataListener = mState->subscribeDataReceived([=](const auto &data) { notifyDataReceived(data); });
			mStatusListener = mState->subscribeStatusReceived([=](const auto &data) { notifyStatusReceived(data); });
			mConnectionListener = mState->subscribeConnectionStateChanged([=](const auto &state) { notifyStateChanged(state, BleDeviceError::NoError); });
			notifyStateChanged(mState->getState(), BleDeviceError::NoError);
		}
		catch (std::exception &e) {
			throw std::runtime_error("Unable connect to device. Device is unreachable");
		}
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
