package com.neuromd.neurosdk.channels;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;

public class SpectrumChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("android-neurosdk");
    }
    
    protected long mNativeObjPtr = 0;
    
    public SpectrumChannel(BaseChannel<Double> channel) {
        mNativeObjPtr = createFromChannel(channel);
        Assert.ensures(mNativeObjPtr != 0,
                "Spectrum channel native object is null");
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

    public native double[] readFast(long offset, long length);
    
    public native double hzPerSpectrumSample();
    
    private static native long createFromChannel(BaseChannel<Double> channel);
    private native void init();
    private native void deleteNative();
}