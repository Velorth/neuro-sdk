#include <CoreBluetooth/CoreBluetooth.h>
#include "ble/mac/macos_ble_enumerator.h"
#include "ble/advertisement_data.h"
#include "event_notifier.h"
#include "string_utils.h"
#include "logger.h"
#include <unordered_set>

using namespace std::chrono_literals;

namespace Neuro {

struct MacOsBleEnumerator::Impl final {
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

MacOsBleEnumerator::MacOsEnumerator(const std::vector<std::string> &name_filters) :
	mImpl(std::make_unique<Impl>(name_filters)){}

MacOsEnumerator::MacOsEnumerator(MacOsEnumerator&&) noexcept = default;

MacOsEnumerator& MacOsEnumerator::operator=(MacOsEnumerator&&) noexcept = default;

void MacOsEnumerator::swap(MacOsEnumerator &other) noexcept {
	using std::swap;
	swap(mImpl, other.mImpl);
}

MacOsEnumerator::~MacOsEnumerator() = default;

ListenerPtr<void, const AdvertisementData&> 
MacOsEnumerator::subscribeAdvertisementReceived(const std::function<void(const AdvertisementData&)>& callback){
	return mImpl->mAdvertiseNotifier.addListener(callback);
}

MacOsEnumerator make_ble_enumerator(const std::vector<std::string> &uuid_filters) {
	return MacOsEnumerator(uuid_filters);
}

void swap(MacOsEnumerator& lhs, MacOsEnumerator& rhs) noexcept {
	lhs.swap(rhs);
}


}
