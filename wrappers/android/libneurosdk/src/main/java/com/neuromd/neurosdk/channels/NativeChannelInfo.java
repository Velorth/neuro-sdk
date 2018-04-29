package com.neuromd.neurosdk.channels;

import com.neuromd.common.Assert;

public class NativeChannelInfo implements ChannelInfo {
    static {
        System.loadLibrary("neurosdk");
    }

    private long mNativeObjPtr = 0;

    private NativeChannelInfo(long ptr){
        mNativeObjPtr = ptr;
        Assert.ensures(mNativeObjPtr != 0, "Channel info pointer is null");
    }

    @Override
    public native String getName();

    @Override
    public native void setName(String name);

    @Override
    public native ChannelType getType();
    
    @Override
    public native long getIndex();
}
