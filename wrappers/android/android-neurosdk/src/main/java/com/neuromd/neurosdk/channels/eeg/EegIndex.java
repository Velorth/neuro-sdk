package com.neuromd.extensions.channels.eeg;

public final class EegIndex {
    public static final EegIndex Alpha = new EegIndex ( "Alpha", 8.f, 14.f );
    public static final EegIndex Beta = new EegIndex ( "Beta", 14.f, 34.f);
    public static final EegIndex Delta = new EegIndex ( "Delta", 0.5f, 4.f );
    public static final EegIndex Theta = new EegIndex ( "Theta", 4.f, 8.f );

    private final String mName;
    private final float mFrequencyBottom;
    private final float mFrequencyTop;

    public EegIndex(String name, float bottomFrequency, float topFrequency){
        mName = name;
        mFrequencyBottom = bottomFrequency;
        mFrequencyTop = topFrequency;
    }

    public String name() {
        return mName;
    }

    public float frequencyBottom() {
        return mFrequencyBottom;
    }

    public float frequencyTop() {
        return mFrequencyTop;
    }
}
