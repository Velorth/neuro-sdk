package com.neuromd.neurosdk.parameters.types;

public final class StimulatorDeviceState {
    public enum State{
        NoParams,
        Disabled,
        Enabled
    }

    private State mStimulatorState;
    private State mMAState;

    public StimulatorDeviceState(State stimulatorState, State maState){
        mStimulatorState = stimulatorState;
        mMAState = maState;
    }

    public State stimulatorState(){
        return mStimulatorState;
    }

    public State maState(){
        return mMAState;
    }
}
