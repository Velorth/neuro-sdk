package ru.neurotech.neurosdk.channels;

import ru.neurotech.neurosdk.Device;

public class DummyChannel extends BaseChannel<Integer> {



    public DummyChannel(BatteryChannel battery){

    }

    @Override
    public ChannelInfo info() {
        return null;
    }

    @Override
    public Integer[] readData(long offset, long length) {
        return new Integer[0];
    }

    @Override
    public long totalLength() {
        return 0;
    }

    @Override
    public long availableLength() {
        return 0;
    }

    @Override
    public float samplingFrequency() {
        return 0;
    }

    @Override
    public void setSamplingFrequency(float frequency) {

    }

    @Override
    public Device underlyingDevice() {
        return null;
    }
}
