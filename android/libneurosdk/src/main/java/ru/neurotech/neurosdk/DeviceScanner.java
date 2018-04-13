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

package ru.neurotech.neurosdk;

import android.content.Context;

import ru.neurotech.common.SubscribersNotifier;

/**
 * Provides common methods for device scanning and
 */
public class DeviceScanner {
    private final long mNativeObjPtr;

    static {
        System.loadLibrary("neurosdk");
    }

    public SubscribersNotifier<NeuroDevice> deviceFound = new SubscribersNotifier<>();

    /**
     * Notifies about changing device scanning state
     */
    public final SubscribersNotifier<Boolean> scanStateChanged = new SubscribersNotifier<>();

    /**
     * Creates new instance of DeviceScanner class with current application context
     *
     * @param context Current application context
     */
    public DeviceScanner(Context context) {
        mNativeObjPtr = createNeuroConnectionObj(context);
    }

    /**
     * Starts Neurotech© devices scanning. Scan stops after @timeout milliseconds
     *
     * @param timeout Duration of scanning. Less or equal to zero for infinity
     */
    public void startScan(int timeout) {
        startScan(mNativeObjPtr, timeout);
    }

    /**
     * Stops device scan process
     */
    public void stopScan() {
        stopScan(mNativeObjPtr);
    }

    public NeuroDevice getDeviceByAddress(String address){
        return findDeviceByAddress(mNativeObjPtr, address);
    }

    private void onDeviceFound(NeuroDevice device) {
        deviceFound.sendNotification(this, device);
    }
    private void onScanStateChanged(boolean isScanning) {
        scanStateChanged.sendNotification(this, isScanning);
    }

    private native long createNeuroConnectionObj(Context appContext);

    private native void deleteNeuroConnectionObj(long objPtr);

    private native void startScan(long objPtr, int timeout);

    private native void stopScan(long objPtr);

    private native NeuroDevice findDeviceByAddress(long objPtr, String address);
}
