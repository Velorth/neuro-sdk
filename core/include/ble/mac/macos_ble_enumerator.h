#ifndef MACOS_BLE_ENUMERATOR_H
#define MACOS_BLE_ENUMERATOR_H

#include "event_listener.h"
#include "lib_export.h"

namespace Neuro {

struct AdvertisementData;

class MacOsBleEnumerator final {
public:
	SDK_SHARED explicit MacOsBleEnumerator(const std::vector<std::string> &name_filters);
	MacOsBleEnumerator(const MacOsBleEnumerator &) = delete;
	MacOsBleEnumerator& operator=(const MacOsBleEnumerator &) = delete;
	SDK_SHARED MacOsBleEnumerator(MacOsBleEnumerator &&) noexcept;
	SDK_SHARED MacOsBleEnumerator& operator=(MacOsBleEnumerator &&) noexcept;
	SDK_SHARED void swap(MacOsBleEnumerator &) noexcept;
	SDK_SHARED ~MacOsBleEnumerator();

	SDK_SHARED ListenerPtr<void, const AdvertisementData &>
	subscribeAdvertisementReceived(const std::function<void(const AdvertisementData &)> &callback);

private:
	struct Impl;
	std::unique_ptr<Impl> mImpl;
};

SDK_SHARED MacOsBleEnumerator make_ble_enumerator(const std::vector<std::string> &name_filters);

SDK_SHARED void swap(MacOsBleEnumerator &lhs, MacOsBleEnumerator &rhs) noexcept;

}
#endif
