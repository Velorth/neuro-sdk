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


ListenerPtr<void, const std::vector<int> &>
DeviceImpl::subscribeBatteryDataReceived(std::function<void(const std::vector<int> &)> callback){
    return mBatteryNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<signal_sample_t> &>
DeviceImpl::subscribeSignalDataReceived(std::function<void(const std::vector<signal_sample_t> &)> callback){
    return mSignalNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<resistance_sample_t> &>
DeviceImpl::subscribeResistanceDataReceived(std::function<void(const std::vector<resistance_sample_t> &)> callback){
    return mResistanceNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<MEMS> &>
DeviceImpl::subscribeMEMSDataReceived(std::function<void(const std::vector<MEMS> &)> callback){
    return mMEMSNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<Quaternion> &>
DeviceImpl::subscribeOrientationDataReceived(std::function<void(const std::vector<Quaternion> &)> callback){
    return mOrientationNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<double> &>
DeviceImpl::subscribeRespirationDataReceived(std::function<void(const std::vector<double> &)> callback){
    return mRespirationNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<int> &>
DeviceImpl::subscribeConnectionStatsDataReceived(std::function<void(const std::vector<int> &)> callback){
    return mConnectionStatsNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<int> &>
DeviceImpl::subscribePedometerDataReceived(std::function<void(const std::vector<int> &)> callback){
    return mPedometerNotifier.addListener(callback);
}

ListenerPtr<void, const std::vector<ElectrodeState> &>
DeviceImpl::subscribeElectrodesDataReceived(std::function<void(const std::vector<ElectrodeState> &)> callback){
    return mElectrodesNotifier.addListener(callback);
}

void DeviceImpl::subscribeDataReceived() {
    Expects(mBleDevice != nullptr);
    mBleDevice->setDataReceivedCallback([=](auto&& data) {
        mDataReceivedQueue.exec([=](){
            onDataReceived(std::forward<decltype(data)>(data));
        });
    });
}

void DeviceImpl::subscribeStatusReceived() {
    Expects(mBleDevice != nullptr);
    mBleDevice->setStatusReceivedCallback([=](auto&& data) {
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
