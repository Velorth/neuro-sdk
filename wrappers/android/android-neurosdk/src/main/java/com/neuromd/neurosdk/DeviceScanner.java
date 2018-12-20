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

package com.neuromd.neurosdk;

import android.content.Context;

import com.neuromd.common.Assert;
import com.neuromd.common.SubscribersNotifier;

/**
 * Provides common methods for device scanning and
 */
public class DeviceScanner {
    private long mNativeObjPtr;

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
        mNativeObjPtr = create(context);
        Assert.ensures(mNativeObjPtr != 0,
                "Device scanner native object is null");
        init();
    }

    public void finalize() throws Throwable {
        if (mNativeObjPtr != 0) {
            deleteNative();
            mNativeObjPtr = 0;
        }
        super.finalize();
    }

    /**
     * Starts Neurotech© devices scanning. Scan stops after @timeout milliseconds
     *
     * @param timeout Duration of scanning. Less or equal to zero for infinity
     */
    public native void startScan(int timeout);

    /**
     * Stops device scan process
     */
    public native void stopScan();

    public native Device findDeviceByAddress(String address);

    private native long create(Context appContext);

    private native void init();

    private native void deleteNative();
}
