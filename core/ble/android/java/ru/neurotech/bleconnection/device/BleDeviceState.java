package ru.neurotech.bleconnection.device;

public enum BleDeviceState {
    DISCONNECTED(0),
    CONNECTED(1),
    ERROR(2);

    private final int mStateCode;

    BleDeviceState(int stateCode){
        mStateCode = stateCode;
    }

    public int getIntCode(){
        return mStateCode;
    }
}
