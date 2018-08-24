package com.neuromd.neurosdk.channels;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;

public class MEMSChannel extends BaseChannel<MEMS> {
    static {
        System.loadLibrary("android-neurosdk");
    }

    protected long mNativeObjPtr = 0;

    public MEMSChannel(Device device) {
        mNativeObjPtr = create(device);
        Assert.ensures(mNativeObjPtr != 0,
                "MEMS channel native object is null");
        init();
    }

    public void finalize() throws Throwable {
        deleteNative();
        super.finalize();
    }

    @Override
    public native ChannelInfo info();

    @Override
    public native MEMS[] readData(long offset, long length);

    @Override
    public native long totalLength();

    @Override
    public native long bufferSize();

    @Override
    public native float samplingFrequency();

    @Override
    public native void setSamplingFrequency(float frequency);

    @Override
    public native Device underlyingDevice();

    private static native long create(Device device);
    private native void init();
    private native void deleteNative();
}