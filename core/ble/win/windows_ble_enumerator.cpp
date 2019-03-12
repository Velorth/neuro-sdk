#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"
#include "ble/win/windows_ble_enumerator.h"
#include "ble/advertisement_data.h"
#include "event_notifier.h"
#include "string_utils.h"
#include "logger.h"
#include <unordered_set>

using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcherStatus;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementFilter;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisement;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEScanningMode;
using winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementType;
using namespace std::chrono_literals;

namespace Neuro {

struct WindowsBleEnumerator::Impl final {
	static constexpr const char *class_name = "WindowsBleEnumerator";
	std::unordered_set<std::string> mFilters;
	Notifier<void, const AdvertisementData &> mAdvertiseNotifier;
	std::mutex mStopScanMutex;
	std::condition_variable mStopScanCondition;
	BluetoothLEAdvertisementWatcher mWatcher;
	winrt::event_token mReceivedEventToken;
	winrt::event_token mStoppedEventToken;

	explicit Impl(const std::vector<std::string> &name_filters) :
		mFilters(name_filters.begin(), name_filters.end()),
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
		mWatcher.Start();
	}

	Impl(const Impl &) = delete;
	Impl& operator=(const Impl &) = delete;
	Impl(Impl &&) = delete;
	Impl& operator=(Impl &&) = delete;

	~Impl() {
		try {
			std::unique_lock<std::mutex> stopLock(mStopScanMutex);		
			const auto status = mWatcher.Status();
			if (status != BluetoothLEAdvertisementWatcherStatus::Started)
				return;

			if (mReceivedEventToken) {
				mWatcher.Received(mReceivedEventToken);
			}
			mWatcher.Stop();
			if (mStopScanCondition.wait_for(stopLock, 3s) != std::cv_status::no_timeout) {
				LOG_ERROR("Failed to destroy WindowsBleEnumerator properly: stop scan timeout");
			}
		}
		catch (std::exception &e) {
			LOG_ERROR_V("Failed to destroy WindowsBleEnumerator properly: %s", e.what());
		}
		catch (...) {
			LOG_ERROR("Failed to destroy WindowsBleEnumerator properly: Unknown error");
		}
	}

	void onAdvertisementReceived(const BluetoothLEAdvertisementWatcher&, const BluetoothLEAdvertisementReceivedEventArgs& args) {
		if (args.AdvertisementType() != BluetoothLEAdvertisementType::ConnectableUndirected)
			return;

		const auto name = to_narrow(args.Advertisement().LocalName().c_str());
		if (mFilters.find(name) == mFilters.end())
			return;

		AdvertisementData advertisementData;
		advertisementData.Name = name;
		advertisementData.Address = static_cast<BleDeviceAddress>(args.BluetoothAddress());
		advertisementData.RSSI = args.RawSignalStrengthInDBm();
		const auto services = args.Advertisement().ServiceUuids();
		advertisementData.ServicesUUIDs = std::vector<GUID>(begin(services), end(services));
		advertisementData.TimeStamp = args.Timestamp();
		mAdvertiseNotifier.notifyAll(advertisementData);
	}
};

static std::vector<GUID> make_guids_from_strings(const std::vector<std::string> &guid_strings) {
	std::vector<GUID> guidList(guid_strings.size());
	std::transform(guid_strings.begin(), guid_strings.end(), guidList.begin(), [](const auto &guid_string) {
		return guid_from_string(guid_string);
	});
	return guidList;
}

WindowsBleEnumerator::WindowsBleEnumerator(const std::vector<std::string> &name_filters) :
	mImpl(std::make_unique<Impl>(name_filters)){}

WindowsBleEnumerator::WindowsBleEnumerator(WindowsBleEnumerator&&) noexcept = default;

WindowsBleEnumerator& WindowsBleEnumerator::operator=(WindowsBleEnumerator&&) noexcept = default;

WindowsBleEnumerator::~WindowsBleEnumerator() = default;

ListenerPtr<void, const AdvertisementData&> 
WindowsBleEnumerator::subscribeAdvertisementReceived(const std::function<void(const AdvertisementData&)>& callback){
	return mImpl->mAdvertiseNotifier.addListener(callback);
}

WindowsBleEnumerator make_ble_enumerator(const std::vector<std::string> &uuid_filters) {
	winrt::init_apartment();
	return WindowsBleEnumerator(uuid_filters);
}


}
