package com.neuromd.extensions.channels.eeg;

public class EmotionalState {
    public int State;
    public int Stress;
    public int Attention;
    public int Relax;
    public int Meditation;
    
    public EmotionalState(int state, int stress, int attention, int relax, int meditation){
        State = state;
        Stress = stress;
        Attention = attention;
        Relax = relax;
        Meditation = meditation;
    }
}
