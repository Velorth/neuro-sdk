package com.neuromd.extensions.channels.ecg;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.channels.BaseChannel;
import com.neuromd.neurosdk.channels.ChannelInfo;
import com.neuromd.neurosdk.channels.ElectrodesStateChannel;
import com.neuromd.neurosdk.channels.SignalChannel;

public class RPeakChannel extends BaseChannel<Long>
{
    static {
        System.loadLibrary("android-sdk-ext");
    }
    
    protected long mNativeObjPtr = 0;
    
    public RPeakChannel(SignalChannel signalChannel){
        mNativeObjPtr = createFromSignalChannel(signalChannel);
        Assert.ensures(mNativeObjPtr != 0,
                "RPeak channel native object is null");
        init();
    }
    
    public RPeakChannel(SignalChannel signalChannel, ElectrodesStateChannel electrodeChannel){
        mNativeObjPtr = createFromSignalAndElectrodeChannel(signalChannel, electrodeChannel);
        Assert.ensures(mNativeObjPtr != 0,
                "RPeak channel native object is null");
        init();
    }
    
    public void finalize() throws Throwable {
        deleteNative();
        super.finalize();
    }
    
    @Override
    public native ChannelInfo info();
    
    @Override
    public native Long[] readData(long offset, long length);
    
    @Override
    public native long totalLength();

    
    @Override
    public native float samplingFrequency();

    private static native long createFromSignalChannel(SignalChannel signalChannel);
    
    private static native long createFromSignalAndElectrodeChannel(SignalChannel signalChannel,
                                                                   ElectrodesStateChannel electrodeChannel);
    
    private native void init();
    
    private native void deleteNative();
}