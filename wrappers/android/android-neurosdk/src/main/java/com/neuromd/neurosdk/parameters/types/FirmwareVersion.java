package com.neuromd.neurosdk.parameters.types;

public final class FirmwareVersion {
    private final int mVersion;
    private final int mBuild;

    public FirmwareVersion(int version, int build){
        mVersion = version;
        mBuild = build;
    }

    public int version(){
        return mVersion;
    }

    public int build(){
        return mBuild;
    }
}
