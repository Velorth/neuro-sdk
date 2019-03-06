#ifndef WINDOWS_BLE_ENUMERATOR_H
#define WINDOWS_BLE_ENUMERATOR_H

#include "event_listener.h"
#include "lib_export.h"

namespace Neuro {

struct AdvertisementData;

class WindowsBleEnumerator final {
public:
	SDK_SHARED explicit WindowsBleEnumerator(const std::vector<std::string> &name_filters);
	WindowsBleEnumerator(const WindowsBleEnumerator &) = delete;
	WindowsBleEnumerator& operator=(const WindowsBleEnumerator &) = delete;
	SDK_SHARED WindowsBleEnumerator(WindowsBleEnumerator &&) noexcept;
	SDK_SHARED WindowsBleEnumerator& operator=(WindowsBleEnumerator &&) noexcept;
	SDK_SHARED ~WindowsBleEnumerator();

	SDK_SHARED ListenerPtr<void, const AdvertisementData &>
	subscribeAdvertisementReceived(const std::function<void(const AdvertisementData &)> &callback);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

SDK_SHARED WindowsBleEnumerator make_ble_enumerator(const std::vector<std::string> &uuid_filters);

}
#endif