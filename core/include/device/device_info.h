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

#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include "ble/platform_traits.h"
#include <string>

namespace Neuro {

struct DeviceInfo {
    std::string Name;
    DeviceAddressType Address;
};

inline bool operator==(const DeviceInfo &lhs, const DeviceInfo &rhs) {
	return lhs.Address == rhs.Address;
}

template <typename Device>
struct DeviceTraits;

}

template <>
struct std::hash<Neuro::DeviceInfo> {
	size_t operator()(const Neuro::DeviceInfo &info) const noexcept {
		using std::hash;
		return hash<Neuro::DeviceAddressType>()(info.Address);
	}
};

#endif