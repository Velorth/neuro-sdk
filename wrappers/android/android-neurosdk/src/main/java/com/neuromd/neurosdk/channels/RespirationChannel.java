package com.neuromd.neurosdk.channels;

import com.neuromd.neurosdk.Device;

public class RespirationChannel extends BaseDoubleChannel {
    static {
        System.loadLibrary("android-neurosdk");
    }

    private final Device mDevice; //store device reference to prevent its deletion
    private final AnyChannel mAnyChannel;
    private final DoubleDataChannel mDataChannel;

    public RespirationChannel(Device device) {
        mDevice = device;
        mAnyChannel = new AnyChannel(createRespirationDoubleChannel(device.devicePtr()), new AnyChannelLengthChangedCallback() {
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
        return RespirationDoubleChannelGetBufferSize(channelPtr());
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

    private static native long createRespirationDoubleChannel(long devicePtr);
    private static native long RespirationDoubleChannelGetBufferSize(long respChannelPtr);
}
