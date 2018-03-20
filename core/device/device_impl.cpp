#include "gsl/gsl_assert"
#include "device/device_impl.h"
#include "device/parameter_reader.h"
#include "device/parameter_writer.h"
#include "ble/ble_device.h"

namespace Neuro {

DeviceImpl::~DeviceImpl(){}

void DeviceImpl::connect(){
    Expects(mBleDevice != nullptr);
    mBleDevice->connect();
}

void DeviceImpl::disconnect(){
    Expects(mBleDevice != nullptr);
    mBleDevice->disconnect();
}

void DeviceImpl::subscribeDataReceived() {
    mBleDevice->setDataReceivedCallback([=](auto&& data) {
        onDataReceived(std::forward<decltype(data)>(data));
    });
}

void DeviceImpl::subscribeStatusReceived() {
    mBleDevice->setStatusReceivedCallback([=](auto&& status) {
        onStatusDataReceived(std::forward<decltype(status)>(status));
    });
}

DeviceImpl::DeviceImpl(std::shared_ptr<BleDevice> ble_device,
                       std::unique_ptr<ParameterReader> reader,
                       std::unique_ptr<ParameterWriter> writer) :
    mBleDevice(ble_device),
    mParamReader(std::move(reader)),
    mParamWriter(std::move(writer)){
    Ensures(mBleDevice != nullptr);
    Ensures(mParamReader != nullptr);
    Ensures(mParamWriter != nullptr);
    subscribeStatusReceived();
    subscribeDataReceived();
}

}
