package ru.neurotech.neurosdk.channels;

import ru.neurotech.neurosdk.Device;

public class BatteryChannel extends NativeChannel<Integer> {
    private BatteryChannel(Device device) {
        super(create(device));
    }

    @Override
    public native Integer[] readData(long offset, long length);

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
