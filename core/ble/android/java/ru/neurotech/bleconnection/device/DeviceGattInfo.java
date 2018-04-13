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

import android.util.Log;

import java.util.UUID;

/**
 * Wrapper class for native gatt information class
 * Stores pointer to native object with BLE Gatt information
 * about found device
 */
public class DeviceGattInfo {
    private long mNativeObjectPtr;

    public DeviceGattInfo(long nativeObjectPtr) {
        mNativeObjectPtr = nativeObjectPtr;
    }

    /**
     * Returns UUID for main service of BLE device
     *
     * @return service UUID
     */
    public UUID deviceServiceUUID() {
        return uuidFromString(getDeviceServiceUUID(mNativeObjectPtr));
    }

    /**
     * Returns UUID for receive data characteristic of device service
     *
     * @return rx UUID
     */
    public UUID rxCharacteristicUUID() {
        return uuidFromString(getRxCharacteristicUUID(mNativeObjectPtr));
    }

    /**
     * Returns data transmission characteristic UUID of device service
     *
     * @return tx UUID
     */
    public UUID txCharacteristicUUID() {
        return uuidFromString(getTxCharacteristicUUID(mNativeObjectPtr));
    }

    /**
     * Returns device status characteristic UUID of device service
     *
     * @return tx UUID
     */
    public UUID statusCharacteristicUUID() {
        return uuidFromString(getStatusCharacteristicUUID(mNativeObjectPtr));
    }

    /**
     * Returns UUID of standard BLE service, which provides common information
     * about device
     *
     * @return generic access service UUID
     */
    public UUID genericAccessServiceUUID() {
        return uuidFromString(getGenericAccessServiceUUID(mNativeObjectPtr));
    }

    public UUID CCCD() {
        return uuidFromString(getCCCD(mNativeObjectPtr));
    }

    private UUID uuidFromString(String uuidString) {
        if (uuidString == null || uuidString.isEmpty()) return null;

        UUID uuid;
        try {
            uuid = UUID.fromString(uuidString);
        } catch (IllegalArgumentException exc) {
            Log.e("DeviceGattInfoWrapper", "Unable to parse UUID: " + exc.getMessage());
            return null;
        }

        return uuid;
    }

    private native String getDeviceServiceUUID(long deviceInfoPtr);

    private native String getRxCharacteristicUUID(long deviceInfoPtr);

    private native String getTxCharacteristicUUID(long deviceInfoPtr);

    private native String getStatusCharacteristicUUID(long deviceInfoPtr);

    private native String getGenericAccessServiceUUID(long deviceInfoPtr);

    private native String getCCCD(long deviceInfoPtr);
}
