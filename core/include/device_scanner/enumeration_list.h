#ifndef ENUMERATION_LIST_H
#define ENUMERATION_LIST_H

#include <memory>
#include <functional>
#include "device/device_info.h"
#include "event_listener.h"

namespace Neuro {

class EnumerationList final {
public:
	explicit EnumerationList(BleTimeDuration refresh_period);
	~EnumerationList();
	std::vector<DeviceInfo> devices() const;
	ListenerPtr<void> subscribeDeviceListChanged(std::function<void()> &callback) const;
	void onAdvertisementReceived(const AdvertisementData &advertisement);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}
#endif // ENUMERATION_LIST_H
