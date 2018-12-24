package com.neuromd.neurosdk.parameters.types;

public final class MotionAssistantParams {
    private int mGyroStart;
    private int mGyroStop;
    private MotionAssistantLimb mLimb;
    private int mMinPause;

    public MotionAssistantParams(int gyroAngleStart,
                                 int gyroAngleStop,
                                 MotionAssistantLimb limb,
                                 int minPause){
        mGyroStart = gyroAngleStart;
        mGyroStop = gyroAngleStop;
        mLimb = limb;
        mMinPause = minPause;
    }

    public int gyroStartAngle(){
        return mGyroStart;
    }

    public int gyroStopAngle(){
        return mGyroStop;
    }

    public MotionAssistantLimb limb(){
        return mLimb;
    }

    public int minPause(){
        return mMinPause;
    }

    public void setGyroStartAngle(int gyroStart){
        mGyroStart = gyroStart;
    }

    public void setGyroStopAngle(int gyroStop){
        mGyroStop = gyroStop;
    }

    public void setLimb(MotionAssistantLimb limb){
        mLimb = limb;
    }

    public void setMinPause(int minPause){
        mMinPause = minPause;
    }
}
