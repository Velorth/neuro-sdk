package ru.neurotech.neurosdk.channels;

import ru.neurotech.neurosdk.Device;

public class SignalChannel extends NativeChannel<Double> {
    private SignalChannel(Device device) {
        super(create(device));
    }

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
}
