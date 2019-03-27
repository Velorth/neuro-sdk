#include <CoreBluetooth/CoreBluetooth.h>
#include "ble/mac/macos_ble_enumerator.h"
#include "ble/ios/ble_delegate.h"
#include "ble/advertisement_data.h"
#include "event_notifier.h"
#include "string_utils.h"
#include "logger.h"
#include <unordered_set>

namespace Neuro {
    
struct MacOsBleEnumerator::Impl final {
	static constexpr const char *class_name = "MacOSBleEnumerator";
	std::unordered_set<std::string> mFilters;
	Notifier<void, const AdvertisementData &> mAdvertiseNotifier;
	std::mutex mStopScanMutex;
	std::condition_variable mStopScanCondition;
    ScannerDelegate* mScanResultDelegate;
	CBCentralManager* mCentralManager;
    

	explicit Impl(const std::vector<std::string> &name_filters) :
		mFilters(name_filters.begin(), name_filters.end()),
        mScanResultDelegate([[ScannerDelegate alloc] initWithDeviceFoundCallback:^(CBPeripheral *peripheral, NSNumber *rssi){
            onAdvertisementReceived(peripheral, rssi);
        }]),
		mCentralManager([[CBCentralManager alloc] initWithDelegate:mScanResultDelegate queue:dispatch_queue_create("central_queue", 0)]) {
		[mCentralManager scanForPeripheralsWithServices:nil options:nil];
	}

	Impl(const Impl &) = delete;
	Impl& operator=(const Impl &) = delete;
	Impl(Impl &&) = delete;
	Impl& operator=(Impl &&) = delete;

	~Impl() {
        if ([mCentralManager isScanning]){
            [mCentralManager stopScan];
        }
	}

	void onAdvertisementReceived(CBPeripheral *peripheral, NSNumber *rssi) {
        auto name = std::string([[peripheral name] UTF8String]);
        if (mFilters.find(name) == mFilters.end())
			return;

		AdvertisementData advertisementData;
		advertisementData.Name = name;
		advertisementData.Address = [[[peripheral identifier] UUIDString] cStringUsingEncoding: NSUTF8StringEncoding];
		advertisementData.RSSI = [rssi intValue];
		advertisementData.ServicesUUIDs = std::vector<std::string>();
        advertisementData.TimeStamp = std::chrono::system_clock::now();
		mAdvertiseNotifier.notifyAll(advertisementData);
	}
};

MacOsBleEnumerator::MacOsBleEnumerator(const std::vector<std::string> &name_filters) :
	mImpl(std::make_unique<Impl>(name_filters)){}

MacOsBleEnumerator::MacOsBleEnumerator(MacOsBleEnumerator&&) noexcept = default;

MacOsBleEnumerator& MacOsBleEnumerator::operator=(MacOsBleEnumerator&&) noexcept = default;

void MacOsBleEnumerator::swap(MacOsBleEnumerator &other) noexcept {
	using std::swap;
	swap(mImpl, other.mImpl);
}

MacOsBleEnumerator::~MacOsBleEnumerator() = default;

ListenerPtr<void, const AdvertisementData&> 
MacOsBleEnumerator::subscribeAdvertisementReceived(const std::function<void(const AdvertisementData&)>& callback){
	return mImpl->mAdvertiseNotifier.addListener(callback);
}

MacOsBleEnumerator make_ble_enumerator(const std::vector<std::string> &uuid_filters) {
	return MacOsBleEnumerator(uuid_filters);
}

void swap(MacOsBleEnumerator& lhs, MacOsBleEnumerator& rhs) noexcept {
	lhs.swap(rhs);
}


}
