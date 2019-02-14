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

package com.neuromd.bleconnection.scanner;

import android.annotation.TargetApi;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.os.Build;
import android.util.Log;

import com.neuromd.bleconnection.exceptions.BluetoothAdapterException;


@TargetApi(Build.VERSION_CODES.LOLLIPOP)
public class BleDeviceScanner21Api extends BleDeviceScanner {
    private ScanCallback mScanCallback;

    public BleDeviceScanner21Api() {
        Log.v("BleDeviceScanner21Api", "Created scanner for API 21 and higher");
        mScanCallback = new ScanCallback() {
            @Override
            public void onScanResult(int callbackType, ScanResult result) {
                onDeviceFound(result.getDevice());
            }
        };
    }

    @Override
    public void startScan(Context context) throws BluetoothAdapterException {
        initAdapter(context);
        mBluetoothAdapter.getBluetoothLeScanner().startScan(mScanCallback);
    }

    @Override
    public void stopScan() {
        if (mBluetoothAdapter == null) return;

        BluetoothLeScanner scanner = mBluetoothAdapter.getBluetoothLeScanner();
        if (scanner == null) return;

        mBluetoothAdapter.getBluetoothLeScanner().stopScan(mScanCallback);
    }
}
