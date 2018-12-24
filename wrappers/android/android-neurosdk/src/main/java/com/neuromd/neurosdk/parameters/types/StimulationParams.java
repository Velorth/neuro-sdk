package com.neuromd.neurosdk.parameters.types;

public final class StimulationParams {
    private int mAmplitude;
    private int mPulseWidth;
    private int mFrequency;
    private int mStimulDuration;

    public StimulationParams(int amplitude, int pulseWidth, int frequency, int stimulDuration){
        mAmplitude = amplitude;
        mPulseWidth = pulseWidth;
        mFrequency = frequency;
        mStimulDuration = stimulDuration;
    }

    public int amplitude(){
        return mAmplitude;
    }

    public int pulseWidth(){
        return mPulseWidth;
    }

    public int frequency(){
        return mFrequency;
    }

    public int stimulusDuration(){
        return mStimulDuration;
    }

    public void setAmplitude(int amplitude){
        mAmplitude = amplitude;
    }

    public void setPulseWidth(int pulseWidth){
        mPulseWidth = pulseWidth;
    }

    public void setFrequency(int frequency){
        mFrequency = frequency;
    }

    public void setStimulusDuration(int stimulDuration){
        mStimulDuration = stimulDuration;
    }
}
