package com.neuromd.neurosdk.channels;

final class DoubleDataChannel {
    private final AnyChannel mAnyChannel;

    DoubleDataChannel(AnyChannel anyChannel){
        mAnyChannel = anyChannel;
    }

    double[] readData(long offset, long length){
        return DoubleChannelReadData(mAnyChannel.channelPtr(), offset, length);
    }

    public static native double[] DoubleChannelReadData(long doubleChannelPtr, long offset, long length);
}
