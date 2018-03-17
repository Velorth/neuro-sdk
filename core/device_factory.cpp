#include "gsl/gsl_assert"
#include "device/device_factory.h"
#include "device/device.h"
#include "ble/ble_device.h"
#include "device/callibri/callibri_impl.h"
#include "device/brainbit/brainbit_impl.h"
#include "device/request_handler.h"
#include "device/callibri/callibri_common_parameters.h"
#include "channels/channel_info.h"

namespace Neuro {

std::unique_ptr<Device> DeviceFactory::createFromImpl(std::unique_ptr<DeviceImpl> impl){
    Expects(impl != nullptr);
    auto device = std::unique_ptr<Device>(new Device(std::move(impl)));
    Ensures(device != nullptr);
    return device;
}


std::unique_ptr<Device> CallibriDeviceFactory::create(std::unique_ptr<BleDevice> ble_device){
    Expects(ble_device != nullptr);
    auto sharedDevice = std::shared_ptr<BleDevice>(ble_device.release());
    auto requestHandler = std::make_shared<CallibriRequestHandler>();
    auto commonParams = std::make_shared<CallibriCommonParameters>(requestHandler);
    auto callibriImpl = std::make_unique<CallibriImpl>(sharedDevice, requestHandler, commonParams);
    auto device = createFromImpl(std::move(callibriImpl));
    Ensures(device != nullptr);
    return device;
}

std::unique_ptr<Device> BrainbitDeviceFactory::create(std::unique_ptr<BleDevice> ble_device){
    Expects(ble_device != nullptr);
    auto sharedDevice = std::shared_ptr<BleDevice>(ble_device.release());
    auto brainbitImpl = std::make_unique<BrainbitImpl>(sharedDevice);
    auto device = createFromImpl(std::move(brainbitImpl));
    Ensures(device != nullptr);
    return device;
}


}
