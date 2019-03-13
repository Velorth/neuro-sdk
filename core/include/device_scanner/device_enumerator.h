/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neuromd.com/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DEVICE_ENUMERATOR_H
#define DEVICE_ENUMERATOR_H

#include "device/device_info.h"
#include "enumeration_list.h"
#include "event_listener.h"

namespace Neuro {

template <typename Device>
class DeviceEnumerator final {
public:
	using DeviceType = Device;

	explicit DeviceEnumerator(BleEnumerator &&enumerator) : 
		mEnumerator(std::move(enumerator)),
		mAdvertiseListenerHandle(subscribeAdvertisement()){}

	DeviceEnumerator(const DeviceEnumerator &) = delete;
	DeviceEnumerator& operator=(const DeviceEnumerator &) = delete;

	DeviceEnumerator(DeviceEnumerator &&rhs) noexcept :
		mDeviceList(std::move(rhs.mDeviceList)),
		mEnumerator(std::move(rhs.mEnumerator)),
		mAdvertiseListenerHandle(subscribeAdvertisement()){
		rhs.mAdvertiseListenerHandle = nullptr;
	}

	DeviceEnumerator& operator=(DeviceEnumerator &&rhs) noexcept {
		this->swap(rhs);
		return *this;
	}

	void swap(DeviceEnumerator &other) noexcept {
		using std::swap;
		swap(this->mDeviceList, other.mDeviceList);
		swap(this->mEnumerator, other.mEnumerator);
		this->mAdvertiseListenerHandle = this->subscribeAdvertisement();
		other.mAdvertiseListenerHandle = other.subscribeAdvertisement();
	}

	~DeviceEnumerator() = default;

	std::vector<DeviceInfo> devices() const {
		return mDeviceList.devices();
	}

	template <typename VoidCallable>
	ListenerPtr<void> subscribeDeviceListChanged(VoidCallable&& callback) {
		return mDeviceList.subscribeListChanged(std::forward<VoidCallable>(callback));
	}

private:	
	EnumerationList mDeviceList;
	BleEnumerator mEnumerator;
	ListenerPtr<void, const AdvertisementData &> mAdvertiseListenerHandle;

	ListenerPtr<void, const AdvertisementData &> subscribeAdvertisement() {
		return mEnumerator.subscribeAdvertisementReceived(
			[=](const AdvertisementData &advertisement) {
				mDeviceList.onAdvertisementReceived(advertisement);
			}
		);
	}
};

template <typename Device, typename... PlatformArgs>
DeviceEnumerator<Device> make_device_enumerator(PlatformArgs&&... args){
	return DeviceEnumerator<Device>(make_ble_enumerator(DeviceTraits<Device>::validNames(), std::forward<PlatformArgs>(args)...));
}

template <typename Device>
void swap(DeviceEnumerator<Device> &lhs, DeviceEnumerator<Device> &rhs) noexcept {
	lhs.swap(rhs);
}

}

#endif