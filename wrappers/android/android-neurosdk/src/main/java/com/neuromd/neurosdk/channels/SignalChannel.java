package com.neuromd.neurosdk.channels;

import com.neuromd.neurosdk.Device;


/**
 * Built-in channel containing signal samples of one physical channel of device
 */
public class SignalChannel extends BaseDoubleChannel {
    static {
        System.loadLibrary("android-neurosdk");
    }

    private final Device mDevice; //store device reference to prevent its deletion
    private final AnyChannel mAnyChannel;
    private final DoubleDataChannel mDataChannel;
    private final AnyChannelLengthChangedCallback lengthChangedCallback = new AnyChannelLengthChangedCallback() {
        @Override
        public void onDataLengthChanged(long dataLength) {
            dataLengthChanged.sendNotification(this, dataLength);
        }
    };

    public SignalChannel(Device device)
    {
        mDevice = device;
        mAnyChannel = new AnyChannel(createSignalDoubleChannel(device.devicePtr()), lengthChangedCallback);
        mDataChannel = new DoubleDataChannel(mAnyChannel);
    }

    public SignalChannel(Device device, ChannelInfo info)
    {
        mDevice = device;
        mAnyChannel = new AnyChannel(createSignalDoubleChannelInfo(device.devicePtr(), info), lengthChangedCallback);
        mDataChannel = new DoubleDataChannel(mAnyChannel);
    }

    public SignalChannel(Device device, ChannelInfo info, Filter[] filters)
    {
        mDevice = device;
        mAnyChannel = new AnyChannel(createSignalDoubleChannelInfoFilters(device.devicePtr(), info, filters), lengthChangedCallback);
        mDataChannel = new DoubleDataChannel(mAnyChannel);
    }

    public SignalChannel(Device device, Filter[] filters)
    {
        mDevice = device;
        mAnyChannel = new AnyChannel(createSignalDoubleChannelFilters(device.devicePtr(), filters), lengthChangedCallback);
        mDataChannel = new DoubleDataChannel(mAnyChannel);
    }

    public ChannelInfo info(){
        return mAnyChannel.info();
    }

    public long totalLength() {
        return mAnyChannel.totalLength();
    }

    public long bufferSize() {
        return SignalDoubleChannelGetBufferSize(channelPtr());
    }

    public float samplingFrequency(){
        return mAnyChannel.samplingFrequency();
    }

    public long channelPtr(){
        return mAnyChannel.channelPtr();
    }

    public double[] readData(long offset, long length) {
        return mDataChannel.readData(offset, length);
    }

    private static native long createSignalDoubleChannel(long devicePtr);
    private static native long createSignalDoubleChannelInfo(long devicePtr, ChannelInfo info);
    private static native long createSignalDoubleChannelInfoFilters(long devicePtr, ChannelInfo info, Filter[] filtersArray);
    private static native long createSignalDoubleChannelFilters(long devicePtr, Filter[] filtersArray);
    private static native long SignalDoubleChannelGetBufferSize(long signalChannelPtr);
}
