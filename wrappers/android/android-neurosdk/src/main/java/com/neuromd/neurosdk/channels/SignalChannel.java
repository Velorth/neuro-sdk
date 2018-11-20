package com.neuromd.neurosdk.channels;

import com.neuromd.common.Assert;
import com.neuromd.neurosdk.Device;


/**
 * Built-in channel containing signal samples of one physical channel of device
 */
public class SignalChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("android-neurosdk");
    }

    protected long mNativeObjPtr = 0;

    public SignalChannel(Device device) {
        mNativeObjPtr = create(device);
        Assert.ensures(mNativeObjPtr != 0,
                "Signal channel native object is null");
        init();
    }

    public SignalChannel(Device device, ChannelInfo info) {
        mNativeObjPtr = createWithInfo(device, info);
        Assert.ensures(mNativeObjPtr != 0,
                "Signal channel native object is null");
        init();
    }

    public SignalChannel(Device device, Filter[] filters) {
        mNativeObjPtr = createWithFilters(device, filters);
        Assert.ensures(mNativeObjPtr != 0,
                "Signal channel native object is null");
        init();
    }

    public SignalChannel(Device device, Filter[] filters, ChannelInfo info) {
        mNativeObjPtr = createWithFiltersAndInfo(device, filters, info);
        Assert.ensures(mNativeObjPtr != 0,
                "Signal channel native object is null");
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

    public native Device underlyingDevice();

    public native double[] readFast(long offset, long length);
    
    private static native long create(Device device);
    private static native long createWithInfo(Device device, ChannelInfo info);
    private static native long createWithFilters(Device device, Filter[] filters);
    private static native long createWithFiltersAndInfo(Device device, Filter[] filters, ChannelInfo info);
    private native void init();
    private native void deleteNative();
}
