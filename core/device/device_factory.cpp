#include "device/device_factory.h"
#include "ble/ble_device.h"
#include "device/callibri/callibri_impl.h"
#include "device/brainbit/brainbit_impl.h"
#include "device/request_scheduler.h"
#include "device/callibri/callibri_common_parameters.h"
#include "channels/info/channel_info.h"

namespace Neuro {

std::unique_ptr<Device, DeviceDeleter> DeviceFactory::createFromImpl(std::unique_ptr<DeviceImpl> impl){
    auto device = std::unique_ptr<Device, DeviceDeleter>(new Device(std::move(impl)), &libDeviceDeleter);
    return device;
}

std::unique_ptr<Device, DeviceDeleter> CallibriDeviceFactory::create(std::unique_ptr<BleDevice> ble_device){
    auto sharedDevice = std::shared_ptr<BleDevice>(ble_device.release());
    auto requestHandler = std::make_shared<CallibriRequestScheduler>();
    auto commonParams = std::make_shared<CallibriCommonParameters>(requestHandler);
    auto callibriImpl = std::make_unique<CallibriImpl>(sharedDevice, requestHandler, commonParams);
    auto device = createFromImpl(std::move(callibriImpl));
    return device;
}

std::unique_ptr<Device, DeviceDeleter> BrainbitDeviceFactory::create(std::unique_ptr<BleDevice> ble_device){
    auto sharedDevice = std::shared_ptr<BleDevice>(ble_device.release());
    auto brainbitImpl = std::make_unique<BrainbitImpl>(sharedDevice);
    auto device = createFromImpl(std::move(brainbitImpl));
    return device;
}

}
