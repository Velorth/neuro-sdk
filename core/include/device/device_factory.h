#ifndef DEVICE_FACTORY_H
#define DEVICE_FACTORY_H

#include <memory>

namespace Neuro {

class BleDevice;
class Device;
class DeviceImpl;

class DeviceFactory{
public:
    virtual std::unique_ptr<Device> create(std::unique_ptr<BleDevice>) = 0;

protected:
    std::unique_ptr<Device> createFromImpl(std::unique_ptr<DeviceImpl>);
};

class CallibriDeviceFactory : public DeviceFactory {
public:
    std::unique_ptr<Device> create(std::unique_ptr<BleDevice>) override;
};

class BrainbitDeviceFactory : public DeviceFactory {
public:
    std::unique_ptr<Device> create(std::unique_ptr<BleDevice>) override;
};

}

#endif // DEVICE_FACTORY_H
