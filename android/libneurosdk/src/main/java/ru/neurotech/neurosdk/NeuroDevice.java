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

import android.util.Log;

import ru.neurotech.common.VisualizableDevice;
import ru.neurotech.neurosdk.features.SignalSubsystem;
import ru.neurotech.neurosdk.features.StimulationSubsystem;
import ru.neurotech.neurosdk.state.NeuroDeviceError;
import ru.neurotech.neurosdk.state.NeuroDeviceState;

/**
 * Represents Neurotech™ device on a layer of algorithmic functionality
 * Provides signal and misc channels and methods for signal processing and viewing
 */
public class NeuroDevice extends VisualizableDevice {
    private long mNativeObjPtr = 0;

    private NeuroDevice(long nativeObjPtr) {
        mNativeObjPtr = nativeObjPtr;
    }

    public void finalize() throws Throwable {
        if (mNativeObjPtr != 0) {
            Log.d("NeuroDevice", "Finalizing");
            deleteDevice(mNativeObjPtr);
            Log.d("NeuroDevice", "Finalized");
            mNativeObjPtr = 0;
        }
        super.finalize();
    }

    /**
     * Returns common device state. To get error info call getError method
     *
     * @return device state
     */
    public NeuroDeviceState getState() {
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        return getDeviceState(mNativeObjPtr);
    }

    /**
     * Returns device error state information
     *
     * @return error type
     */
    public NeuroDeviceError getError() {
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        // return getDeviceError(mNativeObjPtr);
        return NeuroDeviceError.NO_ERROR;
    }

    /**
     * Returns name of BLE device
     *
     * @return BLE device name
     */
    @Override
    public String getName() {
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        return getDeviceName(mNativeObjPtr);
    }

    @Override
    public String getAddress(){
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        return getDeviceAddress(mNativeObjPtr);
    }

    /**
     * Returns battery state
     * @return battery charge level in percents
     */
    @Override
    public int getBatteryLevel(){
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        return getBatteryLevel(mNativeObjPtr);
    }

    @Override
    public NeuroDevice getNeuroDevice() {
        return this;
    }

    /**
     * Returns supported device features
     *
     * @return device feature
     */
    public DeviceFeature[] getFeatures() {
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        return getFeatures(mNativeObjPtr);
    }

    /**
     * Establishes connection with device services
     */
    public void connect() {
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        connectDevice(mNativeObjPtr);
    }

    /**
     * Disconnect from device
     */
    public void disconnect() {
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        disconnectFromDevice(mNativeObjPtr);
    }

    public void close(){
        if (mNativeObjPtr == 0)
            throw new RuntimeException("NeuroDevice object been disposed");

        closeDevice(mNativeObjPtr);
    }

    private native void deleteDevice(long objPtr);

    private native void connectDevice(long objPtr);

    private native void disconnectFromDevice(long objPtr);

    private native void closeDevice(long objPtr);

    private native String getDeviceName(long objPtr);

    private native String getDeviceAddress(long objPtr);

    private native int getBatteryLevel(long objPtr);

    private native NeuroDeviceState getDeviceState(long objPtr);

    private native NeuroDeviceError getDeviceError(long objPtr);

    private native DeviceFeature[] getFeatures(long objPtr);

    private native SignalSubsystem getSignalSubsystem(long objPtr);

    private native StimulationSubsystem getStimulationSubsystem(long objPtr);
}
