package ru.neurotech.neurosdk.channels;

import ru.neurotech.common.Assert;
import ru.neurotech.neurosdk.Device;


/**
 * Built-in channel containing signal samples of one physical channel of device
 */
public class SignalChannel extends BaseChannel<Double> {
    static {
        System.loadLibrary("neurosdk");
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
    public native long bufferSize();

    @Override
    public native float samplingFrequency();

    @Override
    public native void setSamplingFrequency(float frequency);

    @Override
    public native Device underlyingDevice();

    private static native long create(Device device);
    private static native long createWithInfo(Device device, ChannelInfo info);
    private native void init();
    private native void deleteNative();
}
