package com.neuromd.neurosdk.channels;

import com.neuromd.neurosdk.Device;


/**
 * Built-in channel containing resistance samples of one physical channel of device
 */
public class ResistanceChannel extends BaseDoubleChannel {
    static {
        System.loadLibrary("android-neurosdk");
    }

    private final Device mDevice; //store device reference to prevent its deletion
    private final AnyChannel mAnyChannel;
    private final DoubleDataChannel mDataChannel;

    public ResistanceChannel(Device device, ChannelInfo info) {
        mDevice = device;
        mAnyChannel = new AnyChannel(createResistanceDoubleChannelInfo(device.devicePtr(), info), new AnyChannelLengthChangedCallback() {
            @Override
            public void onDataLengthChanged(long dataLength) {
                dataLengthChanged.sendNotification(this, dataLength);
            }
        });
        mDataChannel = new DoubleDataChannel(mAnyChannel);
    }

    public ChannelInfo info(){
        return mAnyChannel.info();
    }

    public long totalLength() {
        return mAnyChannel.totalLength();
    }

    public long bufferSize() {
        return ResistanceDoubleChannelGetBufferSize(channelPtr());
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

    private static native long createResistanceDoubleChannelInfo(long devicePtr, ChannelInfo info);
    private static native long ResistanceDoubleChannelGetBufferSize(long resistChannelPtr);
}
