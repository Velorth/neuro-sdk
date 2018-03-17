package ru.neurotech.neurosdk.channels;

import ru.neurotech.common.Assert;
import ru.neurotech.neurosdk.Device;

public class SignalChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("neurosdk");
    }

    protected long mNativeObjPtr = 0;

    private SignalChannel(Device device) {
        mNativeObjPtr = create(device);
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

    @Override
    public native long availableLength();

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
