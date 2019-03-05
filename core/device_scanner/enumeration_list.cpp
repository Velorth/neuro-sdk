#include <mutex>
#include <unordered_map>
#include "ble/advertisement_data.h"
#include "device_scanner/enumeration_list.h"
#include "event_notifier.h"

namespace Neuro {

struct EnumerationList::Impl final {
	const BleTimeDuration mRefreshPeriod;
	Notifier<void> mListChangedNotifier;
	mutable std::shared_mutex mDeviceMapMutex;
	std::unordered_map<DeviceInfo, BleTimeType> mDeviceMap;

	explicit Impl(BleTimeDuration refresh_period):
		mRefreshPeriod(refresh_period){}

	auto findFirstOverdue() {
		return std::find_if(mDeviceMap.begin(), mDeviceMap.end(), [=](const auto &device_time_pair) {
			return device_time_pair.second < BleTimeType::clock::now() - mRefreshPeriod;
		});
	}

	void checkTimeStamps()
	{
		std::shared_lock readListLock(mDeviceMapMutex);

		if (mDeviceMap.empty())
			return;

		if (findFirstOverdue() != mDeviceMap.end()) {
			readListLock.unlock();
			std::unique_lock removeOverdueLock(mDeviceMapMutex);

			auto overdue = findFirstOverdue();
			while (overdue != mDeviceMap.end()) {
				mDeviceMap.erase(overdue);
				overdue = findFirstOverdue();
			}			
		}

	}
};

EnumerationList::EnumerationList(BleTimeDuration refresh_period) :
	mImpl(std::make_unique<Impl>(refresh_period)){}

EnumerationList::~EnumerationList() = default;

std::vector<DeviceInfo> EnumerationList::devices() const {
	std::shared_lock readListLock(mImpl->mDeviceMapMutex);
	std::vector<DeviceInfo> deviceList(mImpl->mDeviceMap.size());
	std::transform(
		mImpl->mDeviceMap.begin(), 
		mImpl->mDeviceMap.end(), 
		deviceList.begin(), 
		[](const auto& device_time_pair) {
		return device_time_pair.first;
	});
	return deviceList;
}

ListenerPtr<void> EnumerationList::subscribeDeviceListChanged(std::function<void()> &callback) const {
	return mImpl->mListChangedNotifier.addListener(callback);
}

void EnumerationList::onAdvertisementReceived(const AdvertisementData& advertisement) {
	DeviceInfo deviceInfo;
	deviceInfo.Name = advertisement.Name;
	deviceInfo.Address = advertisement.Address;

	std::unique_lock writeListLock(mImpl->mDeviceMapMutex);
	const auto isNewDevice = mImpl->mDeviceMap.find(deviceInfo) != mImpl->mDeviceMap.end();
	mImpl->mDeviceMap[deviceInfo] = advertisement.TimeStamp;
	
	if (!isNewDevice)
		return;

	mImpl->mListChangedNotifier.notifyAll();
}

}
