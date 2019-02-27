package com.neuromd.neurosdk.channels;

public abstract class BaseDoubleChannel extends BaseChannel {
    public abstract double[] readData(long offset, long length);
}
