#include "ble/win/windows_ble_enumerator.h"
#include "ble/advertisement_data.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"
#include "event_notifier.h"
#include "string_utils.h"

using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus;

namespace Neuro {

struct WindowsBleEnumerator::Impl final {
	Notifier<void, const AdvertisementData &> mAdvertiseNotifier;
	std::mutex mStopScanMutex;
	std::condition_variable mStopScanCondition;
	BluetoothLEAdvertisementWatcher mWatcher;
	winrt::event_token mReceivedEventToken;
	winrt::event_token mStoppedEventToken;

	explicit Impl(const GUID &uuid_filter) :
		mReceivedEventToken(mWatcher.Received([=](auto&& watcher, auto&& args) {
		onAdvertisementReceived(std::forward<decltype(watcher)>(watcher), std::forward<decltype(args)>(args));
	})),
		mStoppedEventToken(mWatcher.Stopped([=](const BluetoothLEAdvertisementWatcher &, auto args) {
		std::unique_lock<std::mutex> stopLock(mStopScanMutex);
		if (mStoppedEventToken) {
			mWatcher.Stopped(mStoppedEventToken);
		}
		mStopScanCondition.notify_all();
	})) {
		mWatcher.AdvertisementFilter().Advertisement().ServiceUuids().Append(uuid_filter);
		mWatcher.Start();
	}

	Impl(const Impl &) = delete;
	Impl& operator=(const Impl &) = delete;
	Impl(Impl &&) = delete;
	Impl& operator=(Impl &&) = delete;

	~Impl() {
		std::unique_lock<std::mutex> stopLock(mStopScanMutex);
		const auto status = mWatcher.Status();
		if (status != BluetoothLEAdvertisementWatcherStatus::Started)
			return;

		if (mReceivedEventToken) {
			mWatcher.Received(mReceivedEventToken);
		}
		mWatcher.Stop();
		mStopScanCondition.wait(stopLock);
	}

	void onAdvertisementReceived(const BluetoothLEAdvertisementWatcher&, const BluetoothLEAdvertisementReceivedEventArgs& args) {
		AdvertisementData advertisementData;
		advertisementData.Name = to_narrow(args.Advertisement().LocalName().c_str());
		advertisementData.Address = args.BluetoothAddress();
		advertisementData.RSSI = args.RawSignalStrengthInDBm();
		const auto services = args.Advertisement().ServiceUuids();
		advertisementData.ServicesUUIDs = std::vector<UUIDType>(begin(services), end(services));
		advertisementData.TimeStamp = args.Timestamp();
		mAdvertiseNotifier.notifyAll(advertisementData);
	}
};

WindowsBleEnumerator::WindowsBleEnumerator(const GUID &uuid_filter) :
	mImpl(std::make_unique<Impl>(uuid_filter)){}

WindowsBleEnumerator::~WindowsBleEnumerator() = default;

ListenerPtr<void, const AdvertisementData&> 
WindowsBleEnumerator::subscribeAdvertisementReceived(const std::function<void(const AdvertisementData&)>& callback){
	return mImpl->mAdvertiseNotifier.addListener(callback);
}

}
