package com.neuromd.neurosdk.channels;

final class IntDataChannel {
    private final AnyChannel mAnyChannel;

    IntDataChannel(AnyChannel anyChannel){
        mAnyChannel = anyChannel;
    }

    int[] readData(long offset, long length){
        return IntChannelReadData(mAnyChannel.channelPtr(), offset, length);
    }

    public static native int[] IntChannelReadData(long intChannelPtr, long offset, long length);
}
