#ifndef WINDOWS_BLE_ENUMERATOR_H
#define WINDOWS_BLE_ENUMERATOR_H

#include <guiddef.h>
#include "event_listener.h"

namespace Neuro {

struct AdvertisementData;

class WindowsBleEnumerator final {
public:
	explicit WindowsBleEnumerator(const GUID &uuid_filter);
	WindowsBleEnumerator(const WindowsBleEnumerator &) = delete;
	WindowsBleEnumerator& operator=(const WindowsBleEnumerator &) = delete;
	WindowsBleEnumerator(WindowsBleEnumerator &&) = default;
	WindowsBleEnumerator& operator=(WindowsBleEnumerator &&) = default;
	~WindowsBleEnumerator();

	ListenerPtr<void, const AdvertisementData &> 
	subscribeAdvertisementReceived(const std::function<void(const AdvertisementData &)> &callback);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

}
#endif