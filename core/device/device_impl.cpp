#include "gsl/gsl_assert"
#include "device/device_impl.h"
#include "device/parameter_reader.h"
#include "device/parameter_writer.h"
#include "ble/ble_device.h"

namespace Neuro {

DeviceImpl::~DeviceImpl(){}

void DeviceImpl::connect(){
    Expects(mParamReader != nullptr);
    mParamReader->requestConnect();
}

void DeviceImpl::disconnect(){
    Expects(mParamReader != nullptr);
    mParamReader->requestDisconnect();
}

void DeviceImpl::subscribeDataReceived() {
    Expects(mBleDevice != nullptr);
    mDataReceivedListener = mBleDevice->subscribeDataReceived([=](auto&& data) {
        mDataReceivedQueue.exec([=](){
            onDataReceived(std::forward<decltype(data)>(data));
        });
    });
}

void DeviceImpl::subscribeStatusReceived() {
    Expects(mBleDevice != nullptr);
    mStatusReceivedListener = mBleDevice->subscribeStatusReceived([=](auto&& data) {
        mStatusReceivedQueue.exec([=](){
            onStatusDataReceived(std::forward<decltype(data)>(data));
        });
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
