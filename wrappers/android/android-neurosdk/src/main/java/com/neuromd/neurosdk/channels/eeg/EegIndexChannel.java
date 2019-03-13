package com.neuromd.extensions.channels.eeg;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.channels.BaseChannel;
import com.neuromd.neurosdk.channels.ChannelInfo;

public final class EegIndexChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("android-sdk-ext");
    }

    protected long mNativeObjPtr = 0;

    public EegIndexChannel(EegIndex index, EegChannel[] channels){
       this(index, channels, 8.0, 0.95);
    }

    public EegIndexChannel(EegIndex index, EegChannel[] channels, EegArtifactChannel artifactChannel){
        this(index, channels, artifactChannel, 8.0, 0.95);
    }

    public EegIndexChannel(EegIndex index, EegChannel[] channels, double windowDurationSeconds, double overlapCoeff){
        mNativeObjPtr = createWithChannels(index, channels, windowDurationSeconds, overlapCoeff);
        Assert.ensures(mNativeObjPtr != 0,
                "EEGIndex channel native object is null");
        init();
    }

    public EegIndexChannel(EegIndex index, EegChannel[] channels, EegArtifactChannel artifactChannel, double windowDurationSeconds, double overlapCoeff){
        mNativeObjPtr = createWithChannelsAndArtifacts(index, channels, artifactChannel, windowDurationSeconds, overlapCoeff);
        Assert.ensures(mNativeObjPtr != 0,
                "EEGIndex channel native object is null");
        init();
    }

    public void finalize() throws Throwable {
        deleteNative();
        super.finalize();
    }

    @Override
    public native ChannelInfo info();

    @Override
    public native Double[] readData(long offset, long length);

    @Override
    public native long totalLength();

    @Override
    public native float samplingFrequency();

    private static native long createWithChannels(EegIndex index, EegChannel[] channels, double windowDurationSeconds, double overlapCoeff);
    private static native long createWithChannelsAndArtifacts(EegIndex index, EegChannel[] channels, EegArtifactChannel artifact_channel, double windowDurationSeconds, double overlapCoeff);
    private native void init();
    private native void deleteNative();
}
