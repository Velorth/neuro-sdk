package ru.neurotech.neurosdk.channels;

public class ManagedChannelInfo implements ChannelInfo {
    private String mName;
    private ChannelType mType;

    public ManagedChannelInfo(ChannelType type){
        mType = type;
        mName = type.name();
    }

    public ManagedChannelInfo(ChannelType type, String name){
        mType = type;
        mName = name;
    }

    public ManagedChannelInfo(ChannelInfo rhs){
        mName = rhs.getName();
        mType = rhs.getType();
    }

    @Override
    public String getName() {
        return mName;
    }

    @Override
    public void setName(String name) {
        mName = name;
    }

    @Override
    public ChannelType getType() {
        return mType;
    }
}
