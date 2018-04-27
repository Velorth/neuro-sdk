package ru.neurotech.neurosdk.channels;

public class ManagedChannelInfo implements ChannelInfo {
    private String mName;
    private final ChannelType mType;
    private final long mIndex;

    public ManagedChannelInfo(ChannelType type){
        mType = type;
        mName = type.name();
        mIndex = 0;
    }

    public ManagedChannelInfo(ChannelType type, String name){
        mType = type;
        mName = name;
        mIndex = 0;
    }
    
    public ManagedChannelInfo(ChannelType type, String name, long index){
        mType = type;
        mName = name;
        mIndex = index;
    }

    public ManagedChannelInfo(ChannelInfo rhs){
        mName = rhs.getName();
        mType = rhs.getType();
        mIndex = rhs.getIndex();
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
    
    @Override
    public long getIndex()    {
        return mIndex;
    }
}
