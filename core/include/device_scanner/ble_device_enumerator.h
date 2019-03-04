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

#ifndef BLE_DEVICE_ENUMERATOR_H
#define BLE_DEVICE_ENUMERATOR_H

#include "device/device_info.h"
#include "event_listener.h"

namespace Neuro {

template <typename Device>
class DeviceEnumerator {
public:
	using DeviceType = Device;

	DeviceEnumerator(BleEnumerator &&enumerator) : 
		mEnumerator(std::move(enumerator)){			
		mEnumerator.setServiceFilter(DeviceTraits<Device>::serviceUUIDString());
		mEnumerator.subscribeAdvertisementReceived([=](const AdvertisementData &advertisement){ 
			mDeviceList.onAdvertiseReceived(advertisement); 
		});
	}

	DeviceEnumerator(const DeviceEnumerator &) = delete;
	DeviceEnumerator& operator=(const DeviceEnumerator &) = delete;

	DeviceEnumerator(DeviceEnumerator &&) = default;
	DeviceEnumerator& operator=(DeviceEnumerator &&) = default;

	std::vector<DeviceInfo> devices() const {
		return mDeviceList.devices();
	}

	ListenerPtr<void> subscribeDeviceListChanged(const std::function<void()> &callback) {
		return mDeviceList.subscribeListChanged(callback);
	}

private:	
	DeviceList mDeviceList;
	BleEnumerator mEnumerator;
};

template <typename Device, typename... PlatformArgs>
DeviceEnumerator make_device_enumerator(PlatformArgs&&... args){
	return DeviceEnumerator(make_ble_enumerator(std::forward<PlatformArgs>(args)...))
}

}

#endif