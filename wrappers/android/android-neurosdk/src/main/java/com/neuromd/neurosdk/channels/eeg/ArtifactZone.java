package com.neuromd.extensions.channels.eeg;

public class ArtifactZone {
    public double Time;
    public double Duration;
    public ArtifactType Type;
    public SourceChannel[] Channels;

    public ArtifactZone(double time, double duration, ArtifactType type, SourceChannel[] channels){
        Time = time;
        Duration = duration;
        Type = type;
        Channels = channels;
    }
}
