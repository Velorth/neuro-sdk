#include <algorithm>
#include "ble/win/ble_scanner_win.h"
#include "ble/win/ble_device_win.h"
#include "logger.h"
#include "string_utils.h"

using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementType;
using winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;

namespace Neuro {

BleScannerWin::BleScannerWin() {
	mStoppedEventToken = mWatcher.Stopped([=](const BleWatcherType &, auto args) {
		mIsScanning.store(false);
	});
}

BleScannerWin::~BleScannerWin() {
	if (mStoppedEventToken) {
		mWatcher.Stopped(mStoppedEventToken);
	}
	if (mReceivedEventToken) {
		mWatcher.Received(mReceivedEventToken);
	}
}

void BleScannerWin::startScan(){
    emulator.startScan();
	mReceivedEventToken = mWatcher.Received([=](auto&& watcher, auto&& args) {
		onAdvertisementReceived(std::forward<decltype(watcher)>(watcher), std::forward<decltype(args)>(args));
	});
	mWatcher.Start();
    mIsScanning.store(true);
}

void BleScannerWin::stopScan(){
    emulator.stopScan();
	if (mReceivedEventToken) {
		mWatcher.Received(mReceivedEventToken);
	}
	mWatcher.Stop();
}

std::unique_ptr<BleDevice> BleScannerWin::getDeviceByAddress(std::string address){
    return emulator.getDeviceByAddress(address);
}

void BleScannerWin::setFilter(std::vector<std::string> nameFilter){
    emulator.setFilter(nameFilter);
    mFilterCollection = nameFilter;
}

void BleScannerWin::subscribeDeviceFound(std::function<void (std::unique_ptr<BleDevice>)> callback){
    emulator.subscribeDeviceFound(callback);
    mDeviceFoundCallback = callback;
}

bool BleScannerWin::isScanning(){
    return emulator.isScanning() || mIsScanning.load();
}

void BleScannerWin::releaseDevice(std::string name, std::string address){
    emulator.releaseDevice(name, address);
    LOG_DEBUG_V("Release device %s [%s]", name.c_str(), address.c_str());
	//mFoundDeviceAddresses.erase(mFoundDeviceAddresses.find(address));
}

void BleScannerWin::onAdvertisementReceived(const BleWatcherType& watcher, const BluetoothLEAdvertisementReceivedEventArgs& args) {
	if (args.AdvertisementType() != BluetoothLEAdvertisementType::ConnectableUndirected)
		return;

	const auto deviceName = to_narrow(args.Advertisement().LocalName().c_str());
	if (std::find(mFilterCollection.begin(), mFilterCollection.end(), deviceName) == mFilterCollection.end())
		return;

	const auto deviceAddress = args.BluetoothAddress();
	if (mFoundDeviceAddresses.find(deviceAddress) == mFoundDeviceAddresses.end()) {
		mFoundDeviceAddresses.insert(deviceAddress);
		this->onDeviceFound(std::make_unique<BleDeviceWin>(deviceAddress, deviceName));
	}
}

void BleScannerWin::onDeviceFound(std::unique_ptr<BleDevice> &&device_ptr) const {
    LOG_DEBUG_V("Found device %s [%s]", device_ptr->getName().c_str(), device_ptr->getNetAddress().c_str());
    if (mDeviceFoundCallback)
        mDeviceFoundCallback(std::move(device_ptr));
}

}
