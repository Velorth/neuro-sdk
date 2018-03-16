package ru.neurotech.neurosdk.channels;

import ru.neurotech.common.SubscribersNotifier;
import ru.neurotech.neurosdk.Device;

public abstract class BaseChannel<SampleType> {
    public final SubscribersNotifier<Long> dataLengthChanged = new SubscribersNotifier<>();

    public abstract ChannelInfo info();
    public abstract SampleType[] readData(long offset, long length);
    public abstract long totalLength();
    public abstract long availableLength();
    public abstract float samplingFrequency();
    public abstract void setSamplingFrequency(float frequency);
    public abstract Device underlyingDevice();
}
