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

public class BleDeviceCallback {
    private long mNativeObjPtr;

    public BleDeviceCallback(long nativeObjPtr) {
        mNativeObjPtr = nativeObjPtr;
    }

    public void onConnected() {
        onConnected(mNativeObjPtr);
    }

    public void onDisconnected() {
        onDisconnected(mNativeObjPtr);
    }

    public void onBluetoothDeviceError(BluetoothDeviceError error) {
        onBluetoothDeviceError(mNativeObjPtr, error.getIntCode());
    }

    public void onDataReceived(byte[] data) {
        onDataReceived(mNativeObjPtr, data, data.length);
    }

    public void onStatusReceived(byte[] statusData) {
        onStatusReceived(mNativeObjPtr, statusData, statusData.length);
    }

    private native void onConnected(long callbackPtr);

    private native void onDisconnected(long callbackPtr);

    private native void onBluetoothDeviceError(long callbackPtr, int errorCode);

    private native void onDataReceived(long callbackPtr, byte[] data, int length);

    private native void onStatusReceived(long callbackPtr, byte[] statusData, int length);
}
