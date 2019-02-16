/*
 * Copyright 2016 - 2019 Neurotech MRC. http://neuromd.com/
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

package com.neuromd.neurosdk;

import android.content.Context;
import android.util.Log;

import com.neuromd.common.Assert;
import com.neuromd.common.SubscribersNotifier;

/**
 * Provides common methods for device scanning and
 */
public class DeviceScanner {
    private final long mScannerPtr;
    private final long mScanStateListenerPtr;
    private final long mDeviceFoundListenerPtr;

    static {
        System.loadLibrary("android-neurosdk");
    }

    public final SubscribersNotifier<Device> deviceFound = new SubscribersNotifier<>();

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
        mScannerPtr = createDeviceScanner(context);
        Assert.ensures(mScannerPtr != 0,
                "Device scanner native object is null");
        mScanStateListenerPtr = scannerSubscribeScanState(mScannerPtr);
        mDeviceFoundListenerPtr = scannerSubscribeDeviceFound(mScannerPtr);
    }

    public void finalize() throws Throwable {
        freeListenerHandle(mDeviceFoundListenerPtr);
        freeListenerHandle(mScanStateListenerPtr);
        scannerDelete(mScannerPtr);
        super.finalize();
    }

    public void startScan(int timeoutMs) {
        scannerStartScan(mScannerPtr, timeoutMs);
    }

    public void startScan() {
        scannerStartScan(mScannerPtr, 0);
    }

    public void stopScan() {
        scannerStopScan(mScannerPtr);
    }

    public boolean isScanning(){
        return scannerIsScanning(mScannerPtr);
    }

    public Device findDeviceByAddress(String address) {
        return new Device(scannerGetDeviceByAddress(mScannerPtr, address));
    }

    private void onDeviceFound(long scannerPtr, long devicePtr) {
        if (scannerPtr != mScannerPtr) return;
        try {
            Device device = new Device(devicePtr);
            deviceFound.sendNotification(this, device);
        } catch (Exception e) {
            Log.e(
                    "DeviceScanner",
                    String.format("Exception in device found callback function: %s", e.getMessage())
            );
        }
    }

    private void onScanStateChanged(long scannerPtr, boolean isScanning) {
        if (scannerPtr != mScannerPtr) return;
        try {
            scanStateChanged.sendNotification(this, isScanning);
        } catch (Exception e) {
            Log.e(
                    "DeviceScanner",
                    String.format("Exception in scan state callback function: %s", e.getMessage())
            );
        }
    }

    private static native void freeListenerHandle(long listenerHandle);
    private static native long createDeviceScanner(Context context);
    private static native void scannerDelete(long scannerPtr);
    private static native void scannerStartScan(long scannerPtr, int timeoutMs);
    private static native void scannerStopScan(long scannerPtr);
    private static native boolean scannerIsScanning(long scannerPtr);
    private native long scannerSubscribeDeviceFound(long scannerPtr);
    private native long scannerSubscribeScanState(long scannerPtr);
    private static native long scannerGetDeviceByAddress(long scannerPtr, String address);
}