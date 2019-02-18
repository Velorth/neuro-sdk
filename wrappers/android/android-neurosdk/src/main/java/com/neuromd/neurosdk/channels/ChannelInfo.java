package com.neuromd.neurosdk.channels;

public class ChannelInfo {
    private String mName;
    private final ChannelType mType;
    private final long mIndex;

    public ChannelInfo(ChannelType type){
        mType = type;
        mName = type.name();
        mIndex = 0;
    }

    public ChannelInfo(ChannelType type, String name){
        mType = type;
        mName = name;
        mIndex = 0;
    }

    public ChannelInfo(ChannelType type, String name, long index){
        mType = type;
        mName = name;
        mIndex = index;
    }

    public ChannelInfo(ChannelInfo rhs){
        mName = rhs.getName();
        mType = rhs.getType();
        mIndex = rhs.getIndex();
    }

    public String getName() {
        return mName;
    }

    public void setName(String name) {
        mName = name;
    }

    public ChannelType getType() {
        return mType;
    }

    public long getIndex()    {
        return mIndex;
    }
}
