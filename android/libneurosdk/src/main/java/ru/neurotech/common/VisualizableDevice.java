package ru.neurotech.common;

import ru.neurotech.neurosdk.NeuroDevice;
import ru.neurotech.neurosdk.state.NeuroDeviceState;

public abstract class VisualizableDevice {

    /**
     * Subscribe this event to get notifications about device state
     */
    public final SubscribersNotifier<NeuroDeviceState> deviceStateChanged = new SubscribersNotifier<>();

    /**
     * Subscribe this event to get notifications about device battery level
     */
    public final SubscribersNotifier<Integer> batteryLevelChanged = new SubscribersNotifier<>();


    /**
     * Returns name of BLE device
     *
     * @return BLE device name
     */
    public abstract String getName();

    /**
     * Returns Bluetooth LE MAC address
     * @return device unique address
     */
    public abstract String getAddress();

    /**
     * Returns battery state
     * @return battery charge level in percents
     */
    public abstract int getBatteryLevel();

    /**
     * Returns physical device object which this visual device represents
     * @return NeuroDevice of current visual device
     */
    public abstract NeuroDevice getNeuroDevice();
}
