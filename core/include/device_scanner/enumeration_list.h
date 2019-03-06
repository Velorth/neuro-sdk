#ifndef ENUMERATION_LIST_H
#define ENUMERATION_LIST_H

#include <memory>
#include <functional>
#include "device/device_info.h"
#include "event_listener.h"

namespace Neuro {

struct AdvertisementData;

class EnumerationList final {
public:
	SDK_SHARED EnumerationList();
	EnumerationList(const EnumerationList &) = delete;
	EnumerationList& operator=(const EnumerationList &) = delete;
	SDK_SHARED EnumerationList(EnumerationList &&) noexcept;
	SDK_SHARED EnumerationList& operator=(EnumerationList &&) noexcept;
	SDK_SHARED ~EnumerationList();
	SDK_SHARED std::vector<DeviceInfo> devices() const;
	SDK_SHARED ListenerPtr<void> subscribeListChanged(const std::function<void()> &callback) const;
	SDK_SHARED void onAdvertisementReceived(const AdvertisementData &advertisement);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}
#endif // ENUMERATION_LIST_H
