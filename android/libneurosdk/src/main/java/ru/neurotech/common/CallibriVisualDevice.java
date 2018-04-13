package ru.neurotech.common;

import android.graphics.Color;

import ru.neurotech.neurosdk.state.NeuroDeviceState;

/**
 * This class is suitable only for Callibri devices
 * @param <T> Visualizable device type
 */
public class CallibriVisualDevice<T extends VisualizableDevice> {
    private String mName;
    private int mColor;
    private T mCallibriDevice;

    public SubscribersNotifier<NeuroDeviceState> DeviceLost = new SubscribersNotifier<>();

    public CallibriVisualDevice(T hardwareDevice) {
        mCallibriDevice = hardwareDevice;
        mCallibriDevice.deviceStateChanged.subscribe(deviceStateChangedCallback);
        String bleName = mCallibriDevice.getName();
        switch (bleName) {
            case "Neurotech_Colibri_R":
            case "Neurotech_Callibri_R":{
                mName = "Callibri Red";
                mColor = Color.RED;
                break;
            }
            case "Neurotech_Colibri_B":
            case "Neurotech_Callibri_B":{
                mName = "Callibri Blue";
                mColor = Color.BLUE;
                break;
            }
            case "Neurotech_Colibri_Y":
            case "Neurotech_Callibri_Y": {
                mName = "Callibri Yellow";
                mColor = Color.YELLOW;
                break;
            }
            case "Neurotech_Colibri_W":
            case "Neurotech_Callibri_W": {
                mName = "Callibri White";
                mColor = Color.WHITE;
                break;
            }
            default:
                throw new DeviceNotSupportedException(String.format("Device %s is not supported", bleName));
        }
    }

    public String getName() {
        return mName;
    }

    public String getAddress(){
        return mCallibriDevice.getAddress();
    }

    public int getBatteryLevel(){
        return mCallibriDevice.getBatteryLevel();
    }

    public int getColor() {
        return mColor;
    }

    public final T getCallibriDevice() {
        return mCallibriDevice;
    }

    private INotificationCallback<NeuroDeviceState> deviceStateChangedCallback = new INotificationCallback<NeuroDeviceState>() {
        @Override
        public void onNotify(Object sender, NeuroDeviceState state) {
            onDeviceStateChanged(state);
        }
    };

    private void onDeviceStateChanged(NeuroDeviceState state) {
        if (state != NeuroDeviceState.CONNECTED && state != NeuroDeviceState.WORKING)
            DeviceLost.sendNotification(this, state);
    }
}
