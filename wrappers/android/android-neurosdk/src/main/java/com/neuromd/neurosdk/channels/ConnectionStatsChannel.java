package com.neuromd.neurosdk.channels;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;

public class ConnectionStatsChannel extends BaseChannel<Integer> {
    static {
        System.loadLibrary("android-neurosdk");
    }

    protected long mNativeObjPtr = 0;

    public ConnectionStatsChannel(Device device) {
        mNativeObjPtr = create(device);
        Assert.ensures(mNativeObjPtr != 0,
                "Connection statistics channel native object is null");
        init();
    }

    public void finalize() throws Throwable {
        deleteNative();
        super.finalize();
    }

    @Override
    public native ChannelInfo info();

    @Override
    public native Integer[] readData(long offset, long length);

    @Override
    public native long totalLength();

    public native long bufferSize();

    @Override
    public native float samplingFrequency();

    public native Device underlyingDevice();

    private static native long create(Device device);
    private native void init();
    private native void deleteNative();
}
