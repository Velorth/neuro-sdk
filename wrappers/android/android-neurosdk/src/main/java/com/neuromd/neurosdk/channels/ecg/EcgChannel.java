package com.neuromd.extensions.channels.ecg;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.channels.BaseChannel;
import com.neuromd.neurosdk.channels.ChannelInfo;
import com.neuromd.neurosdk.channels.SignalChannel;

public class EcgChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("android-sdk-ext");
    }
    
    protected long mNativeObjPtr = 0;

    public EcgChannel(Device device, ChannelInfo info){
        mNativeObjPtr = createWithInfo(device, info);
        Assert.ensures(mNativeObjPtr != 0,
                "ECG channel native object is null");
        init();
    }

    public EcgChannel(Device device){
        mNativeObjPtr = createWithDevice(device);
        Assert.ensures(mNativeObjPtr != 0,
                "ECG channel native object is null");
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

    private static native long createWithInfo(Device device, ChannelInfo info);
    private static native long createWithDevice(Device device);
    private native void init();
    private native void deleteNative();
}
