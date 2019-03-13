package com.neuromd.extensions.channels.ecg;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.channels.BaseChannel;
import com.neuromd.neurosdk.channels.ChannelInfo;

public class StressIndexChannel extends BaseChannel<Double>
{
    static {
        System.loadLibrary("android-sdk-ext");
    }
    
    protected long mNativeObjPtr = 0;
    
    public StressIndexChannel(RPeakChannel rPeakChannel){
        mNativeObjPtr = createFromRPeakChannel(rPeakChannel);
        Assert.ensures(mNativeObjPtr != 0,
                "StressIndex channel native object is null");
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
    
    public native void setSamplingFrequency(float frequency);

    private static native long createFromRPeakChannel(RPeakChannel rPeakChannel);
    
    private native void init();
    
    private native void deleteNative();
}