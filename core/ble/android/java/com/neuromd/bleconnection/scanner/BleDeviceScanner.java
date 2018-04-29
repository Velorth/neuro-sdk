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

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.util.Log;


import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.neuromd.bleconnection.device.DeviceFilter;
import com.neuromd.bleconnection.device.DeviceFoundCallback;
import com.neuromd.bleconnection.exceptions.BluetoothAdapterException;
import com.neuromd.bleconnection.exceptions.BluetoothPermissionException;

/**
 * Parent class for device scanners from different android API
 */
public abstract class BleDeviceScanner {
    /**
     * List of found devices
     * It must be thread safe because onLeScan callback may be called from different threads
     */
    private final List<BluetoothDevice> mDevices = Collections.synchronizedList(new ArrayList<BluetoothDevice>());

    private DeviceFilter mFilter;

    private DeviceFoundCallback mDeviceFoundCallback;

    protected BluetoothAdapter mBluetoothAdapter;

    /**
     * Sets device filter for scan results
     *
     * @param filter device filter
     */
    public void setFilter(DeviceFilter filter) {
        mFilter = filter;
    }

    /**
     * Get list of found devices
     *
     * @return found BrainBit devices list
     */
    public List<BluetoothDevice> getDevices() {
        return mDevices;
    }

    /**
     * Gets count of found BLE devices
     *
     * @return count of found BLE devices
     */
    public int getDeviceCount() {
        return mDevices.size();
    }


    /**
     * Returns reference to bluetooth device object with specified address.
     * Method does not guarantee that physical device with that address is exists
     * and is reachable. Returns null if address string is not valid.
     *
     * @param context Application context to use Bluetooth
     * @param address Device MAC-address
     * @return BluetoothDevice object
     * @throws BluetoothAdapterException
     */
    public BluetoothDevice getDeviceByAddress(Context context, String address) throws BluetoothAdapterException {
        initAdapter(context);
        if (!BluetoothAdapter.checkBluetoothAddress(address)) return null;

        return mBluetoothAdapter.getRemoteDevice(address);
    }

    /**
     * Starts search for ble devices
     *
     * @throws BluetoothPermissionException
     * @throws BluetoothAdapterException
     */
    public abstract void startScan(Context context) throws BluetoothPermissionException, BluetoothAdapterException;

    /**
     * Cancels device search
     */
    public abstract void stopScan();

    /**
     * Resets state of BLE scanner
     * Does not reset established device connections, just clears list of devices,
     * removes filter object and sets current adapter reference to null.
     * You have to keep an eye on devices connection states on your own
     */
    public void reset() {
        Log.v("BleDeviceScanner", "reset");
        mDevices.clear();
    }

    public void subscribeDeviceFound(DeviceFoundCallback callback) {
        mDeviceFoundCallback = callback;
    }

    /**
     * Gets reference to bluetooth adapter from system service and store it in mBluetoothAdapter field
     *
     * @param context app context
     * @throws BluetoothAdapterException
     */
    protected void initAdapter(Context context) throws BluetoothAdapterException {
        Log.v("Scanner", "Init adapter");
        // mDevices.clear();
        //if adapter is not available, try to init it
        if (mBluetoothAdapter != null && mBluetoothAdapter.isEnabled()) return;

        if (mBluetoothAdapter == null) {
            BluetoothManager bluetoothManager = (BluetoothManager) context.getSystemService(Context.BLUETOOTH_SERVICE);
            mBluetoothAdapter = bluetoothManager.getAdapter();
        }

        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled())
            throw new BluetoothAdapterException("Bluetooth adapter initialization failure");
    }

    /**
     * This method will be called by derivative classes when any device is found
     *
     * @param device found device
     */
    protected void onDeviceFound(final BluetoothDevice device) {
        /** You should know
         * this method probably could be called in different JNI threads simultaneously
         */
        if (device == null || device.getName() == null) return;
        Log.v("Scanner", String.format("Found device: %s", device.getName()));

        if (mFilter != null) {
            //Skip device add and notification if it's not device we're looking for
            if (!mFilter.filter(device.getName())) return;
        }

        //Now we should check if we've already added device with the same name and address
        //This been made because of different behavior of le scanners in android 6.0 for
        //different devices. Sometimes leScanCallback hits multiple times for the same device
        if (deviceAlreadyFound(device)) return;

        mDevices.add(device);
        if (mDeviceFoundCallback != null) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    mDeviceFoundCallback.onDeviceFound(device);
                }
            }).start();
        }
    }

    /**
     * Checks whether device been found already or not
     *
     * @param newDevice device for check
     * @return True if device already in list
     */
    private boolean deviceAlreadyFound(BluetoothDevice newDevice) {
        if (mDevices.isEmpty()) {
            Log.v("BleDeviceList", "List is empty");
            return false;
        }
        for (BluetoothDevice device : mDevices) {
            if (device.getName().equals(newDevice.getName())) {
                if (device.getAddress().equals(newDevice.getAddress()))
                    return true;
            }
        }
        return false;
    }

    protected void releaseDevice(String name, String address){
        if (mDevices.isEmpty()) {
            Log.v("BleDeviceList", "List is empty");
            return;
        }
        int devIndex = -1;
        for (BluetoothDevice device : mDevices) {
            if (device.getName().equals(name)) {
                if (device.getAddress().equals(address)) {
                    devIndex = mDevices.indexOf(device);
                    break;
                }
            }
        }
        if (devIndex >= 0){
            mDevices.remove(devIndex);
        }
    }
}
