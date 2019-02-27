package com.neuromd.neurosdk.channels;

public class SpectrumChannel extends BaseDoubleChannel {
    static {
        System.loadLibrary("android-neurosdk");
    }

    private final BaseChannel mChannel; //store device reference to prevent its deletion
    private final AnyChannel mAnyChannel;
    private final DoubleDataChannel mDataChannel;

    public SpectrumChannel(BaseDoubleChannel channel) {
        mChannel = channel;
        mAnyChannel = new AnyChannel(createSpectrumDoubleChannel(mChannel.channelPtr()), new AnyChannelLengthChangedCallback() {
            @Override
            public void onDataLengthChanged(long dataLength) {
                dataLengthChanged.sendNotification(this, dataLength);
            }
        });
        mDataChannel = new DoubleDataChannel(mAnyChannel);
    }

    public ChannelInfo info(){
        return mAnyChannel.info();
    }

    public long totalLength() {
        return mAnyChannel.totalLength();
    }

    public float samplingFrequency(){
        return mAnyChannel.samplingFrequency();
    }

    public long channelPtr(){
        return mAnyChannel.channelPtr();
    }

    public double[] readData(long offset, long length) {
        return mDataChannel.readData(offset, length);
    }

    public double hzPerSpectrumSample() {
        return SpectrumDoubleChannelGetHzPerSpectrumSample(mAnyChannel.channelPtr());
    }

    public long spectrumLength() {
        return SpectrumDoubleChannelGetSpectrumLength(mAnyChannel.channelPtr());
    }

    private static native long createSpectrumDoubleChannel(long channelPtr);
    private static native double SpectrumDoubleChannelGetHzPerSpectrumSample(long spectrumChannelPtr);
    private static native long SpectrumDoubleChannelGetSpectrumLength(long spectrumChannelPtr);
}