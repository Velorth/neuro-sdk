package ru.neurotech.neurosdk.channels;

import ru.neurotech.neurosdk.Device;

public abstract class NativeChannel<SampleType> extends BaseChannel<SampleType> {
    static {
        System.loadLibrary("neurosdk");
    }

    protected long mNativeObjPtr = 0;

    protected NativeChannel(long mObjPtr){
        mNativeObjPtr = mObjPtr;
        init();
    }

    public native ChannelInfo info();
    public abstract SampleType[] readData(long offset, long length);
    public abstract long totalLength();
    public abstract long availableLength();
    public abstract float samplingFrequency();
    public abstract void setSamplingFrequency(float frequency);
    public abstract Device underlyingDevice();

    private native void init();
}
