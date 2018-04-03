package ru.neurotech.neurosdk.channels;

import ru.neurotech.common.Assert;
import ru.neurotech.neurosdk.Device;

public class AngleChannel extends BaseChannel<Quaternion>{
    static {
        System.loadLibrary("neurosdk");
    }

    protected long mNativeObjPtr = 0;

    public AngleChannel(Device device) {
        mNativeObjPtr = create(device);
        Assert.ensures(mNativeObjPtr != 0,
                "Angle channel native object is null");
        init();
    }

    public void finalize() throws Throwable {
        deleteNative();
        super.finalize();
    }

    @Override
    public native ChannelInfo info();

    @Override
    public native Quaternion[] readData(long offset, long length);

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
