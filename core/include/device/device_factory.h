#ifndef DEVICE_FACTORY_H
#define DEVICE_FACTORY_H

#include <memory>
#include "device/device.h"

namespace Neuro {

class BleDevice;

class DeviceFactory{
public:
    virtual std::unique_ptr<Device, DeviceDeleter> create(std::unique_ptr<BleDevice>) = 0;

protected:
    std::unique_ptr<Device, DeviceDeleter> createFromImpl(std::unique_ptr<DeviceImpl>);
};

class CallibriDeviceFactory : public DeviceFactory {
public:
    std::unique_ptr<Device, DeviceDeleter> create(std::unique_ptr<BleDevice>) override;
};

class BrainbitDeviceFactory : public DeviceFactory {
public:
    std::unique_ptr<Device, DeviceDeleter> create(std::unique_ptr<BleDevice>) override;
};

}

#endif // DEVICE_FACTORY_H
