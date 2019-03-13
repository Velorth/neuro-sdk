package com.neuromd.extensions.channels.eeg;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.channels.BaseChannel;
import com.neuromd.neurosdk.channels.ChannelInfo;

public class EegChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("android-sdk-ext");
    }
    
    protected long mNativeObjPtr = 0;
    
    public EegChannel(Device device, ChannelInfo info){
        mNativeObjPtr = createWithInfo(device, info);
        Assert.ensures(mNativeObjPtr != 0,
                "EEG channel native object is null");
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
    
    public native long bufferSize();
    
    @Override
    public native float samplingFrequency();

    public native double[] readFast(long offset, long length);
    
    private static native long createWithInfo(Device device, ChannelInfo info);
    private native void init();
    private native void deleteNative();
}
