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

package ru.neurotech.bleconnection.scanner;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.util.Log;

import ru.neurotech.bleconnection.exceptions.BluetoothAdapterException;
import ru.neurotech.bleconnection.exceptions.BluetoothPermissionException;


public class BleDeviceScannerOldApi extends BleDeviceScanner {
    private BluetoothAdapter.LeScanCallback mLeScanHandle;

    public BleDeviceScannerOldApi() {
        Log.v("BleDeviceScannerOldApi", "Created scanner for old API");
        mLeScanHandle = new BluetoothAdapter.LeScanCallback() {
            @Override
            public void onLeScan(BluetoothDevice bluetoothDevice, int rssi, byte[] scanRecord) {
                onDeviceFound(bluetoothDevice);
            }
        };
    }

    @Override
    public void startScan(Context context) throws BluetoothPermissionException, BluetoothAdapterException {
        initAdapter(context);
        mBluetoothAdapter.startLeScan(mLeScanHandle);
    }

    @Override
    public void stopScan() {
        if (mBluetoothAdapter == null) return;

        mBluetoothAdapter.stopLeScan(mLeScanHandle);
    }
}
