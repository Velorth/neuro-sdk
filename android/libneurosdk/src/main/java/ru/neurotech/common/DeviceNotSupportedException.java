package ru.neurotech.common;


public class DeviceNotSupportedException extends RuntimeException {
    public DeviceNotSupportedException() {
    }

    public DeviceNotSupportedException(String message) {
        super(message);
    }
}
