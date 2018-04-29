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

import com.neuromd.common.Assert;
import com.neuromd.common.SubscribersNotifier;
import com.neuromd.neurosdk.channels.ChannelInfo;
import com.neuromd.neurosdk.parameters.Command;
import com.neuromd.neurosdk.parameters.Parameter;
import com.neuromd.neurosdk.parameters.ParameterName;
import com.neuromd.neurosdk.parameters.types.DeviceState;

public class Device {
    static {
        System.loadLibrary("neurosdk");
    }

    private long mNativeObjPtr = 0;

    private Device(long nativeObjPtr) {
        mNativeObjPtr = nativeObjPtr;
        Assert.ensures(mNativeObjPtr != 0, "Device pointer is null");
        init();
    }

    public void finalize() throws Throwable {
        if (mNativeObjPtr != 0) {
            deleteDevice();
            mNativeObjPtr = 0;
        }
        super.finalize();
    }

    /**
     * Subscribe this event to receive notifications about changes of device parameters
     */
    public final SubscribersNotifier<ParameterName> parameterChanged = new SubscribersNotifier<>();

    /**
     * Tries to establish connection with device
     * Check DeviceState parameter or subscribe parameterChanged event for operation result
     */
    public native void connect();

    /**
     * Disconnects from device
     * Check DeviceState parameter or subscribe parameterChanged event for operation result
     */
    public native void disconnect();

    /**
     * Returns information about supported channels
     * Check this information before creation of channel. If device does not support channel,
     * channel object won't be initialized with it
     * @return Array of channel info objects
     */
    public native ChannelInfo[] channels();

    /**
     * Returns supported commands of device
     * @return Array of supported commands
     */
    public native Command[] commands();

    /**
     * Returns all available parameters of device, their types and access rights
     * @return Array of available parameters
     */
    public native Parameter[] parameters();

    /**
     * Tries to execute command and returns value indicating operations success. Will throw if
     * device does not support specified command. To get supported commands call commands() method
     * @param cmd Command to execute
     * @return Operation success indicator
     * @throws UnsupportedOperationException
     */
    public native boolean execute(Command cmd);

    /**
     * Return value of specified parameter of device. Will throw if parameter does not present in
     * device. To get supported parameters and type information for parameter call parameters()
     * method. It returns Parameter object which consists of parameter name, type and access mode
     * @param param ParameterName to read
     * @return Parameter value
     * @throws UnsupportedOperationException
     */
    public native <ParamType> ParamType readParam(ParameterName param);

    /**
     * Sets value for specified parameter and returns value indicating success of operation. Will
     * throw if parameter does not present in device or has only Read access mode. To get supported
     * parameters and type information for parameter call parameters() method. It returns Parameter
     * object which consists of parameter name, type and access mode
     * @param param Name of parameter to set
     * @param value Parameter value
     * @return Operation success
     * @throws UnsupportedOperationException
     */
    public native boolean setParam(ParameterName param, Object value);

    private native void init();
    private native void deleteDevice();
}
