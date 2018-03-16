package ru.neurotech.neurosdk.channels;

public class NativeChannelInfo implements ChannelInfo {
    static {
        System.loadLibrary("neurosdk");
    }

    private long mNativeObjPtr = 0;

    private NativeChannelInfo(long ptr){
        mNativeObjPtr = ptr;
    }

    @Override
    public native String getName();

    @Override
    public native void setName(String name);

    @Override
    public native ChannelType getType();
}
