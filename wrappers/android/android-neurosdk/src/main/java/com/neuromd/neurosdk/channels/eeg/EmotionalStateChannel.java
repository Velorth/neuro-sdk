package com.neuromd.extensions.channels.eeg;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;
import com.neuromd.neurosdk.channels.BaseChannel;
import com.neuromd.neurosdk.channels.ChannelInfo;

public class EmotionalStateChannel extends BaseChannel<EmotionalState> {
    static {
        System.loadLibrary("android-sdk-ext");
    }
    
    protected long mNativeObjPtr = 0;
    
    public EmotionalStateChannel(EegChannel t3, EegChannel t4, EegChannel o1, EegChannel o2){
        mNativeObjPtr = createFromEegChannels(t3, t4, o1, o2);
        Assert.ensures(mNativeObjPtr != 0,
                "EmotionalState channel native object is null");
        init();
    }
    
    public void finalize() throws Throwable {
        deleteNative();
        super.finalize();
    }
    
    @Override
    public native ChannelInfo info();
    
    @Override
    public native EmotionalState[] readData(long offset, long length);
    
    @Override
    public native long totalLength();
    
    public native long bufferSize();
    
    @Override
    public native float samplingFrequency();

    public native void reset();
    
    private static native long createFromEegChannels(EegChannel t3, EegChannel t4, EegChannel o1, EegChannel o2);
    private native void init();
    private native void deleteNative();
}
