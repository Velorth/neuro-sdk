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

package com.neuromd.bleconnection.device;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.content.Context;
import android.os.ConditionVariable;
import android.util.Log;

import java.lang.reflect.Method;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Provides usable interface for Neurotech™ BLE devices.
 * Hides usage of Android Bluetooth classes
 */
public class BleDevice {
    private static final int MAX_WRITE_TX_CHARACTERISTIC_ATTEMPTS = 3;
    private static final int WRITE_TX_CHARACTERISTIC_RETRY_DELAY_MS = 300;

    private final String mName;
    private final String mAddress;
    private ConditionVariable mWriteDescriptorCondition = new ConditionVariable(true);
    private ReentrantLock mWriteDescriptorLock = new ReentrantLock();

    private BluetoothGattCallback mBleGattCallback = new BluetoothGattCallback() {
        //WARNING
        //All of these callback methods could be called from different
        //JNI threads simultaneously. It depends on connection quality, forced
        //disconnections caused, for example, by device power off. Bluetooth stack of Android
        //can accumulate messages from devices and then give them to you at one time


        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            //Translating status codes of BluetoothGatt class to elements of our enum
            if (status == BluetoothGatt.GATT_SUCCESS || status == 8) // 8 is for timeout status
                switch (newState) {
                    case BluetoothGatt.STATE_CONNECTING:
                        Log.d("JavaBleDevice", "On connecting");
                        break;
                    case BluetoothGatt.STATE_CONNECTED: {
                        Log.d("JavaBleDevice", "On connected");
                        if (!gatt.discoverServices()) {
                            new Thread(new Runnable() {
                                @Override
                                public void run() {
                                    if (mDeviceCallback != null) {
                                        mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.SERVICE_NOT_FOUND);
                                    }
                                }
                            }).start();
                        }

                        break;
                    }
                    case BluetoothGatt.STATE_DISCONNECTING:
                        Log.d("JavaBleDevice", "On disconnecting");
                        break;
                    case BluetoothGatt.STATE_DISCONNECTED: {
                        Log.d("JavaBleDevice", "On disconnected");
                        mConnectionState = BleDeviceState.DISCONNECTED;
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                if (mDeviceCallback != null) {
                                    mDeviceCallback.onDisconnected();
                                }
                            }
                        }).start();
                        break;
                    }
                    default: {
                        Log.e("JavaBleDevice", "Unknown connection state");
                        mConnectionState = BleDeviceState.ERROR;
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                if (mDeviceCallback != null) {
                                    mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.GENERAL_CONNECTION_ERROR);
                                }
                            }
                        }).start();
                    }
                }
            else {
                Log.e("JavaBleDevice", String.format("Failed change connection state. Error code: %d", status));
                mConnectionState = BleDeviceState.ERROR;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (mDeviceCallback != null) {
                            mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.GENERAL_CONNECTION_ERROR);
                        }
                    }
                }).start();
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            Log.v("JavaBleDevice", "On services discovered");
            if (status != BluetoothGatt.GATT_SUCCESS) {
                mConnectionState = BleDeviceState.ERROR;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (mDeviceCallback != null) {
                            mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.SERVICE_NOT_FOUND);
                        }
                    }
                }).start();
                return;
            }

            final BluetoothGattService deviceService = gatt.getService(mGattInfo.deviceServiceUUID());
            if (deviceService == null) {
                mConnectionState = BleDeviceState.ERROR;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (mDeviceCallback != null) {
                            mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.SERVICE_NOT_FOUND);
                        }
                    }
                }).start();
                return;
            }

            BluetoothGattCharacteristic txCharacteristic = deviceService.getCharacteristic(mGattInfo.txCharacteristicUUID());
            if (txCharacteristic != null) {
                mTxCharacteristic = txCharacteristic;
                Log.v("JavaBleDevice", "On transmit ready");
            } else {
                Log.e("JavaBleDevice", "Transmit characteristic not found");
                mConnectionState = BleDeviceState.ERROR;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (mDeviceCallback != null) {
                            mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.TX_NOT_FOUND);
                        }
                    }
                }).start();
                return;
            }

            new Thread(new Runnable() {
                @Override
                public void run() {
                    BluetoothGattCharacteristic rxCharacteristic = deviceService.getCharacteristic(mGattInfo.rxCharacteristicUUID());
                    if (rxCharacteristic != null) {
                        if (setCharacteristicNotification(rxCharacteristic, true)) {
                            Log.v("JavaBleDevice", "On receive ready");
                        } else {
                            Log.e("JavaBleDevice", "Failed to enable notifications for receive characteristic");
                            mConnectionState = BleDeviceState.ERROR;
                            new Thread(new Runnable() {
                                @Override
                                public void run() {
                                    if (mDeviceCallback != null) {
                                        mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.STATUS_NOT_FOUND);
                                    }
                                }
                            }).start();
                            return;
                        }
                    } else {
                        Log.e("JavaBleDevice", "Receive characteristic not found");
                        mConnectionState = BleDeviceState.ERROR;
                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                if (mDeviceCallback != null) {
                                    mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.RX_NOT_FOUND);
                                }
                            }
                        }).start();
                        return;
                    }


                    //Getting characteristic for devices with status characteristic
                    if (mGattInfo.statusCharacteristicUUID() != null) {
                        BluetoothGattCharacteristic statusCharacteristic = deviceService.getCharacteristic(mGattInfo.statusCharacteristicUUID());
                        if (statusCharacteristic != null) {
                            if (setCharacteristicNotification(statusCharacteristic, true)) {
                                Log.v("JavaBleDevice", "On status ready");
                            } else {
                                Log.e("JavaBleDevice", "Failed to enable notifications for status characteristic");
                                mConnectionState = BleDeviceState.ERROR;
                                if (mDeviceCallback != null)
                                    mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.STATUS_NOT_FOUND);
                                return;
                            }
                        } else {
                            Log.e("JavaBleDevice", "Status characteristic not found");
                            mConnectionState = BleDeviceState.ERROR;
                            if (mDeviceCallback != null)
                                mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.STATUS_NOT_FOUND);
                            return;
                        }
                    }

                    mConnectionState = BleDeviceState.CONNECTED;
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            if (mDeviceCallback != null) mDeviceCallback.onConnected();
                        }
                    }).start();
                }
            }).start();
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) {
            Log.v("JavaBleDevice", String.format("Characteristic onWrite status: %d", status));
            if (status != BluetoothGatt.GATT_SUCCESS) {
                Log.e("JavaBleDevice", "Characteristic write error");
                mConnectionState = BleDeviceState.ERROR;
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (mDeviceCallback != null) {
                            mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.GENERAL_CONNECTION_ERROR);
                        }
                    }
                }).start();
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            if (characteristic.getUuid().equals(mGattInfo.rxCharacteristicUUID())) {
                onReceiveCharacteristicChanged(characteristic);
            } else if (characteristic.getUuid().equals(mGattInfo.statusCharacteristicUUID())) {
                onStatusCharacteristicChanged(characteristic);
            }
        }

        @Override
        public void onDescriptorWrite(BluetoothGatt gatt, BluetoothGattDescriptor descriptor, int status) {
            Log.v("JavaBleDevice", String.format("Descriptor onWrite status: %d", status));
            if (status != BluetoothGatt.GATT_SUCCESS) {
                mConnectionState = BleDeviceState.ERROR;
                Log.e("JavaBleDevice", "Descriptor write response error");
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        if (mDeviceCallback != null) {
                            mDeviceCallback.onBluetoothDeviceError(BluetoothDeviceError.GENERAL_CONNECTION_ERROR);
                        }
                    }
                }).start();
                return;
            }
            Log.v("JavaBleDevice", "Descriptor been written successfully");
            mWriteDescriptorCondition.open();
        }
    };

    /**
     * Stores information about GATT Server of concrete BLE device
     */
    private DeviceGattInfo mGattInfo;

    /**
     * Source bluetooth device
     */
    private final BluetoothDevice mBleDevice;

    /**
     * GATT server instance of source device
     */
    private BluetoothGatt mDeviceGatt;

    /**
     * Transmitt characteristic for sending commands to device
     */
    private BluetoothGattCharacteristic mTxCharacteristic;

    /**
     * BluetoothDevice and Gatt events callback
     */
    private BleDeviceCallback mDeviceCallback;

    private BleDeviceState mConnectionState = BleDeviceState.DISCONNECTED;


    /**
     * RX characteristic changed callback
     *
     * @param rxCharacteristic
     */
    private void onReceiveCharacteristicChanged(BluetoothGattCharacteristic rxCharacteristic) {
        final byte[] data = rxCharacteristic.getValue();
        new Thread(new Runnable() {
            @Override
            public void run() {
                if (mDeviceCallback != null) {
                    mDeviceCallback.onDataReceived(data);
                }
            }
        }).start();
    }

    /**
     * Status characteristic changed callback
     *
     * @param statusCharacteristic
     */
    private void onStatusCharacteristicChanged(BluetoothGattCharacteristic statusCharacteristic) {
        final byte[] statusData = statusCharacteristic.getValue();
        Log.v("JavaBleDevice", "On status characteristic changed");
        new Thread(new Runnable() {
            @Override
            public void run() {
                if (mDeviceCallback != null) {
                    mDeviceCallback.onStatusReceived(statusData);
                }
            }
        }).start();
    }

    /**
     * Enables or disables notifications of specified characteristic
     *
     * @param characteristic bluetooth characteristic
     * @param enabled        flag indicates whether enable or disable notifications of characteristic
     * @return
     */
    private boolean setCharacteristicNotification(final BluetoothGattCharacteristic characteristic, boolean enabled) {
        if (characteristic == null) {
            Log.e("Connectivity", "Gatt characteristic is null");
            return false;
        }

        if (mDeviceGatt == null) {
            Log.e("JavaBleDevice", "Cannot set notifications. Device GATT is null");
            return false;
        }

        boolean result;

        mWriteDescriptorLock.lock();
        boolean chSet = mDeviceGatt.setCharacteristicNotification(characteristic, enabled);
        BluetoothGattDescriptor descriptor = characteristic.getDescriptor(mGattInfo.CCCD());
        if (descriptor == null)
            return false;
        descriptor.setValue(enabled ? BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE : BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE);
        mWriteDescriptorCondition.close();
        boolean descWritten = mDeviceGatt.writeDescriptor(descriptor);
        result = mWriteDescriptorCondition.block(1000) && chSet && descWritten;
        mWriteDescriptorLock.unlock();

        Log.v("BleDevice", String.format("Characteristic descriptor %s", result ? "been written successfully" : "not written"));
        return result;
    }


    /**
     * Initializes base device class from bluetoothDevice instance
     *
     * @param sourceDevice bluetooth device
     * @param context      application context
     */
    public BleDevice(BluetoothDevice sourceDevice, DeviceGattInfo gattInfo) {
        Log.v("JavaBleDevice", "Constructor");
        mBleDevice = sourceDevice;
        mName = mBleDevice.getName();
        mAddress = mBleDevice.getAddress();
        mGattInfo = gattInfo;
    }

    @Override
    public void finalize() throws Throwable {
        Log.d("JavaBleDevice", "Finalizing");
        this.close();
        super.finalize();
    }

    public String name() {

        return mName;
    }

    public String address() {

        return mAddress;
    }

    public BleDeviceState state(){

        return mConnectionState;
    }

    private boolean refreshDeviceCache(BluetoothGatt gatt){
        try {
            Method localMethod = gatt.getClass().getMethod("refresh");
            if (localMethod != null) {
                return (boolean) (Boolean) localMethod.invoke(gatt);
            }
        }
        catch (Exception localException) {
            Log.e("JavaBleDevice", "An exception occurred while refreshing device");
        }
        return false;
    }

    /**
     * Establishes a connection to device
     */
    public void connect(final Context context) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Log.v("JavaBleDevice", "Connecting to device");

                if (mDeviceGatt == null) {
                    if (mBleDevice == null){
                        mConnectionState = BleDeviceState.ERROR;
                        return;
                    }
                    mDeviceGatt = mBleDevice.connectGatt(context, false, mBleGattCallback);
                }
                else
                    mDeviceGatt.connect();
                refreshDeviceCache(mDeviceGatt);
            }
        }).start();
    }

    /**
     * Disconnects from GATT server without releasing any resources.
     * You'll be able to reconnect same server without initializing
     * new GATT instance
     */
    public void disconnect() {
        Log.v("JavaBleDevice", "Disconnecting from device");
        if (mDeviceGatt == null) return;
        mDeviceGatt.disconnect();
    }

    /**
     * Disconnects from GATT server and releases all resources
     */
    public void close() {
        Log.v("JavaBleDevice", "Closing device");
        if (mDeviceGatt == null) return;
        mDeviceGatt.close();
        mDeviceGatt = null;
        mConnectionState = BleDeviceState.ERROR;
    }

    /**
     * Sets callback class for bluetooth notifications
     *
     * @param callback
     */
    public void subscribeDeviceEvents(BleDeviceCallback callback) {
        mDeviceCallback = callback;
    }

    /**
     * Sends command to Neurotech™ BLE device
     *
     * @param cmdArray packet contains command
     * @return flag shows success of characteristic write
     */
    public boolean sendCommand(byte[] cmdArray) {
        if (mTxCharacteristic == null) {
            Log.e("JavaBleDevice", "Cannot send command. Tx characteristic is null");
            return false;
        }

        if (mDeviceGatt == null) {
            Log.e("JavaBleDevice", "Cannot send command. Device GATT is null");
            return false;
        }

        if (mConnectionState!=BleDeviceState.CONNECTED){
            Log.e("JavaBleDevice", "Cannot send command. Device is disconnected");
            return false;
        }

        Log.v("JavaBleDevice", "Sending command");

        mTxCharacteristic.setValue(cmdArray);
        mTxCharacteristic.setWriteType(BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE);

        boolean res = mDeviceGatt.writeCharacteristic(mTxCharacteristic);

        if (res) return true;

        Log.w("JavaBleDevice", "Failed write tx characteristic. Try to rewrite");
        for (int k = 0; !res && k < MAX_WRITE_TX_CHARACTERISTIC_ATTEMPTS; ++k) {
            try {
                Thread.sleep(WRITE_TX_CHARACTERISTIC_RETRY_DELAY_MS);
            } catch (Exception e) {
                Log.w("JavaBleDevice", "Thread sleep raised interrupted exception");
                return false;
            }
            res = mDeviceGatt.writeCharacteristic(mTxCharacteristic);
        }

        if (!res)
            Log.e("JavaBleDevice", "Failed write tx characteristic. Return");

        return res;
    }
}
