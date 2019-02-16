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

public class Device {
    static {
        System.loadLibrary("android-neurosdk");
    }

    private final long mParamChangedListenerPtr;
    private final Dictionary<Action<object, ChannelData<double>>, IntPtr> _doubleChannelListenerHandles = new Dictionary<Action<object, ChannelData<double>>, IntPtr>();
    private final Dictionary<Action<object, ChannelData<int>>, IntPtr> _intChannelListenerHandles = new Dictionary<Action<object, ChannelData<int>>, IntPtr>();

    private final SubscribersNotifier<object, ChannelData<double>> mDoubleChannelDataReceived;
    private final SubscribersNotifier<object, ChannelData<int>> mIntChannelDataReceived;

    private final long mDevicePtr;

    Device(long devicePtr){
        Assert.expects(devicePtr != 0, "Device pointer is null");
        mDevicePtr = devicePtr;
        mParamChangedListenerPtr = deviceSubscribeParamChanged(mDevicePtr);
    }

    public void finalize() throws Throwable {
        RemoveAllCahnnelListeners();
        freeListenerHandle(mParamChangedListenerPtr);
        deviceDelete(mDevicePtr);
        super.finalize();
    }

    /**
     * Subscribe this event to receive notifications about changes of device parameters
     */
    public final SubscribersNotifier<ParameterName> parameterChanged = new SubscribersNotifier<>();

    public long devicePtr(){
        return mDevicePtr;
    }

    /**
     * Tries to establish connection with device
     * Check DeviceState parameter or subscribe parameterChanged event for operation result
     */
    public void connect(){
        deviceConnect(mDevicePtr);
    }

    /**
     * Disconnects from device
     * Check DeviceState parameter or subscribe parameterChanged event for operation result
     */
    public void disconnect(){
        deviceDisconnect(mDevicePtr);
    }

    /**
     * Returns information about supported channels
     * Check this information before creation of channel. If device does not support channel,
     * channel object won't be initialized with it
     * @return Array of channel info objects
     */
    public ChannelInfo[] channels(){
        return deviceAvailableChannels(mDevicePtr);
    }

    /**
     * Returns supported commands of device
     * @return Array of supported commands
     */
    public Command[] commands(){
        return deviceAvailableCommands(mDevicePtr);
    }

    /**
     * Returns all available parameters of device, their types and access rights
     * @return Array of available parameters
     */
    public Parameter[] parameters(){
        return deviceAvailableParameters(mDevicePtr);
    }

    /**
     * Tries to execute command and returns value indicating operations success. Will throw if
     * device does not support specified command. To get supported commands call commands() method
     * @param cmd Command to execute
     * @return Operation success indicator
     * @throws UnsupportedOperationException
     */
    public void execute(Command cmd){
        deviceExecute(mDevicePtr, cmd);
    }

    /**
     * Return value of specified parameter of device. Will throw if parameter does not present in
     * device. To get supported parameters and type information for parameter call parameters()
     * method. It returns Parameter object which consists of parameter name, type and access mode
     * @param param ParameterName to read
     * @return Parameter value
     * @throws UnsupportedOperationException
     */
    public <ParamType> ParamType readParam(ParameterName param){
        ParameterTypeInfo paramTypeInfo = new ParameterTypeInfo(parameter);
        if (paramTypeInfo.Type != typeof(T))
        {
            throw new ArgumentException($"Wrong return generic type argument. Must be {paramTypeInfo.Type.Name}");
        }

        var paramValue = paramTypeInfo.ReadParamValue(this);
        if (!(paramValue is T))
        {
            throw new ArgumentException("Return type of native readParam method is not equal to generic type parameter");
        }

        return (ParamType)paramValue;
    }

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




    public T ReadParam<T>(Parameter parameter)
    {

    }

    public void SetParam<T>(Parameter parameter, T value)
    {
        var paramTypeInfo = new ParameterTypeInfo(parameter);
        if (paramTypeInfo.Type != typeof(T))
        {
            throw new ArgumentException($"Wrong return generic type argument. Must be {paramTypeInfo.Type.Name}");
        }

        paramTypeInfo.SetParamValue(this, value);
    }

    public void AddDoubleChannelDataListener(
            Action<object, ChannelData<double>> callback,
            ChannelInfo channelInfo)
    {
        if (_doubleChannelListenerHandles.ContainsKey(callback))
        {
            freeListenerHandle(_doubleChannelListenerHandles[callback]);
        }

        SdkError.ThrowIfError(deviceSubscribeDoubleChannelDataReceived(DevicePtr, channelInfo,
                _doubleDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

        _doubleChannelListenerHandles[callback] = dataReceivedListenerPtr;
        DoubleChannelDataReceived += callback;
    }

    public void AddIntChannelDataListener(
            Action<object, ChannelData<int>> callback,
            ChannelInfo channelInfo)
    {
        if (_intChannelListenerHandles.ContainsKey(callback))
        {
            freeListenerHandle(_intChannelListenerHandles[callback]);
        }

        SdkError.ThrowIfError(deviceSubscribeIntChannelDataReceived(DevicePtr, channelInfo,
                _intDataReceivedFunc, out var dataReceivedListenerPtr, IntPtr.Zero));

        _intChannelListenerHandles[callback] = dataReceivedListenerPtr;
        IntChannelDataReceived += callback;
    }

    public void RemoveIntChannelDataListener(Action<object, ChannelData<int>> callback)
    {
        if (_intChannelListenerHandles.ContainsKey(callback))
        {
            freeListenerHandle(_intChannelListenerHandles[callback]);
            _intChannelListenerHandles.Remove(callback);
        }
    }

    public void RemoveDoubleChannelDataListener(Action<object, ChannelData<double>> callback)
    {
        if (_doubleChannelListenerHandles.ContainsKey(callback))
        {
            freeListenerHandle(_doubleChannelListenerHandles[callback]);
            _doubleChannelListenerHandles.Remove(callback);
        }
    }

    private void RemoveAllCahnnelListeners()
    {
        foreach (var listener in _intChannelListenerHandles.Values)
        {
            freeListenerHandle(listener);
        }
        _intChannelListenerHandles.Clear();
        foreach (var listener in _doubleChannelListenerHandles.Values)
        {
            freeListenerHandle(listener);
        }
        _doubleChannelListenerHandles.Clear();
    }

    private void onParameterChanged(long devicePtr, ParameterName parameterName){
        if (mDevicePtr != devicePtr) return;
        parameterChanged.sendNotification(this, parameterName);
    }

    private void onDoubleDataReceived(long devicePtr, ChannelInfo info, double[] dataArray){
        if (mDevicePtr != devicePtr) return;
        mDoubleChannelDataReceived.sendNotification(this, new DoubleChannelData(dataArray, info));
    }

    private void onIntDataReceived(long devicePtr, ChannelInfo info, int[] dataArray){
        if (mDevicePtr != devicePtr) return;
        mIntChannelDataReceived.sendNotification(this, new IntChannelData(dataArray, info));
    }

    public class DoubleChannelData {
        private final ChannelInfo mChannelInfo;
        private final double[] mDataArray;

        public ChannelInfo channelInfo() {
            return mChannelInfo;
        }

        public double[] dataArray() {
            return mDataArray;
        }

        public DoubleChannelData(double[] array, ChannelInfo info) {
            mDataArray = array;
            mChannelInfo = info;
        }
    }

    public class IntChannelData {
        private final ChannelInfo mChannelInfo;
        private final int[] mDataArray;

        public ChannelInfo channelInfo() {
            return mChannelInfo;
        }

        public int[] dataArray() {
            return mDataArray;
        }

        public IntChannelData(int[] array, ChannelInfo info) {
            mDataArray = array;
            mChannelInfo = info;
        }
    }

    private static native void freeListenerHandle(long listenerHandle);
    private static native void deviceConnect(long devicePtr);
    private static native void deviceDisconnect(long devicePtr);
    private static native void deviceDelete(long devicePtr);
    private static native ChannelInfo[] deviceAvailableChannels(long devicePtr);
    private static native Command[] deviceAvailableCommands(long devicePtr);
    private static native Parameter[] deviceAvailableParameters(long devicePtr);
    private static native void deviceExecute(long devicePtr, Command command);
    private static native long deviceSubscribeParamChanged(long devicePtr);
    private static native long deviceSubscribeDoubleChannelDataReceived(long devicePtr, ChannelInfo info);
    private static native long deviceSubscribeIntChannelDataReceived(long devicePtr, ChannelInfo info);
}
