package com.neuromd.neurosdk.parameters;

public class Parameter {
    private final ParameterName mName;
    private final ParameterAccess mAccess;
    private final ParameterType mType;

    private Parameter(ParameterName name, ParameterAccess access, ParameterType type){
        mName = name;
        mAccess = access;
        mType = type;
    }

    public ParameterName getName() {
        return mName;
    }

    public ParameterAccess getAccess() {
        return mAccess;
    }

    public ParameterType getType() {
        return mType;
    }
}
