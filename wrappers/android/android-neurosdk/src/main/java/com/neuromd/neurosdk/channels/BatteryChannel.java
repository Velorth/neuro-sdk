package com.neuromd.neurosdk.channels;

import com.neuromd.neurosdk.Device;

/**
 * Built-in channel containing battery charge data
 */
public class BatteryChannel extends BaseChannel {
    static {
        System.loadLibrary("android-neurosdk");
    }

    private final Device mDevice; //store device reference to prevent its deletion
    private final AnyChannel mAnyChannel;
    private final IntDataChannel mDataChannel;

    public BatteryChannel(Device device)
    {
        mDevice = device;
        mAnyChannel = new AnyChannel(createBatteryIntChannel(device.devicePtr()), new AnyChannelLengthChangedCallback() {
            @Override
            public void onDataLengthChanged(long dataLength) {
                dataLengthChanged.sendNotification(this, dataLength);
            }
        });
        mDataChannel = new IntDataChannel(mAnyChannel);
    }

    public ChannelInfo info(){
        return mAnyChannel.info();
    }

    public long totalLength() {
        return mAnyChannel.totalLength();
    }

    public long bufferSize() {
        return BatteryIntChannelGetBufferSize(channelPtr());
    }

    public float samplingFrequency(){
        return mAnyChannel.samplingFrequency();
    }

    public long channelPtr(){
        return mAnyChannel.channelPtr();
    }

    public int[] readData(int offset, int length) {
        return mDataChannel.readData(offset, length);
    }

    private static native long createBatteryIntChannel(long devicePtr);
    private static native long BatteryIntChannelGetBufferSize(long batteryChannelPtr);
}
