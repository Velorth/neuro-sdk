package com.neuromd.neurosdk.channels;

final class AnyChannel {
    private final long mListenerPtr;
    private final long mChannelPtr;
    private final AnyChannelLengthChangedCallback mLengthChangedCallback;

    public long channelPtr() {
        return mChannelPtr;
    }

    public ChannelInfo info() {
        return AnyChannelGetInfo(mChannelPtr);
    }

    public long totalLength() {
        return AnyChannelGetTotalLength(mChannelPtr);
    }

    public float samplingFrequency() {
        return AnyChannelGetSamplingFrequency(mChannelPtr);
    }

    public AnyChannel(long channelPtr, AnyChannelLengthChangedCallback lengthCallback) {
        if (channelPtr == 0) {
            throw new IllegalArgumentException("Channel pointer is null");
        }

        mChannelPtr = channelPtr;
        mLengthChangedCallback = lengthCallback;
        mListenerPtr = AnyChannelSubscribeLengthChanged(mChannelPtr);
    }

    @Override
    protected void finalize() throws Throwable {
        freeListenerHandle(mListenerPtr);
        AnyChannelDelete(mChannelPtr);
        super.finalize();
    }

    private void onTotalLengthChanged(long channelPtr, long length)    {
        if (channelPtr != mChannelPtr) return;

        if (mLengthChangedCallback != null) {
            mLengthChangedCallback.onDataLengthChanged(length);
        }
    }

    private static native void freeListenerHandle(long listenerHandle);

    private static native void AnyChannelDelete(long anyChannelPtr);

    private static native ChannelInfo AnyChannelGetInfo(long anyChannelPtr);

    private static native float AnyChannelGetSamplingFrequency(long anyChannelPtr);

    private native long AnyChannelSubscribeLengthChanged(long anyChannelPtr);

    private static native long AnyChannelGetTotalLength(long anyChannelPtr);
}
