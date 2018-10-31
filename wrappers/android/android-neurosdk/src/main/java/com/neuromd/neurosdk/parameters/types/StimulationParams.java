package com.neuromd.neurosdk.parameters.types;

public final class StimulationParams {
    private int mAmplitude;
    private StimulatorImpulseDuration mPulseDuration;
    private int mFrequency;
    private int mStimulDuration;

    public StimulationParams(int amplitude, StimulatorImpulseDuration pulseDuration, int frequency, int stimulDuration){
        mAmplitude = amplitude;
        mPulseDuration = pulseDuration;
        mFrequency = frequency;
        mStimulDuration = stimulDuration;
    }

    public int amplitude(){
        return mAmplitude;
    }

    public StimulatorImpulseDuration pulseDuration(){
        return mPulseDuration;
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

    public void setPulseDuration(StimulatorImpulseDuration pulseDuration){
        mPulseDuration = pulseDuration;
    }

    public void setFrequency(int frequency){
        mFrequency = frequency;
    }

    public void setStimulusDuration(int stimulDuration){
        mStimulDuration = stimulDuration;
    }
}
