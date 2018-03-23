/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
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

package ru.neurotech.bleconnection.device;

import android.bluetooth.BluetoothDevice;
import android.util.Log;

public class DeviceFoundCallback {
    private long mNativeObjectPtr;

    public DeviceFoundCallback(long subscriberPtr) {

        mNativeObjectPtr = subscriberPtr;
        Log.v("DeviceFoundCallback", String.format("Created callback class. Native ptr is %d", mNativeObjectPtr));
    }

    public void onDeviceFound(BluetoothDevice device) {
        Log.v("DeviceFoundCallback", String.format("Device found: %s [%s]", device.getName(), device.getAddress()));
        notifyDeviceFound(mNativeObjectPtr, device);
    }

    private native void notifyDeviceFound(long subscriberPtr, BluetoothDevice foundDevice);
}
