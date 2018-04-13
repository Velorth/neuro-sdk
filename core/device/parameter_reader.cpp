#include "gsl/gsl_assert"
#include "device/parameter_reader.h"
#include "device/ble_device.h"
#include "logger.h"

namespace Neuro {

ParameterReader::ParameterReader(std::shared_ptr<BleDevice> ble_device):
    mBleDevice(ble_device){
    Ensures(mBleDevice != nullptr);
    subscribeBleDeviceStateChanged();
}

typename ParamValue<Parameter::Name>::Type ParameterReader::readName() const {
    Expects(mBleDevice != nullptr);
    return mBleDevice->getName();
}

ParamValue<Parameter::State>::Type ParameterReader::readState() const {
    return mState;
}

typename ParamValue<Parameter::Address>::Type ParameterReader::readAddress() const {
    Expects(mBleDevice != nullptr);
    return mBleDevice->getNetAddress();
}

void ParameterReader::subscribeBleDeviceStateChanged() {
    bleDeviceStateHandler = MakeHandler(BleDevice, deviceStateChanged,
                                        [=](const BleDevice &,
                                        BleDeviceState state,
                                        BleDeviceError error){
        onBleDeviceStateChanged(state, error);
    });
   mBleDevice->deviceStateChanged += bleDeviceStateHandler;
}

void ParameterReader::onBleDeviceStateChanged(BleDeviceState state, BleDeviceError error) {
    if (state == BleDeviceState::CONNECTED) {
        onBleConnected();
    }
    else {
        onBleDisconnected(error);
    }
}

void ParameterReader::onBleConnected(){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    log->debug("[%s: %s] Ble device connected. Device %s, address: %s. Initializing...", "ParameterReader", __FUNCTION__,
               mBleDevice->getName().c_str(), mBleDevice->getNetAddress().c_str());
    if (loadDeviceParams()) {
        mState = DeviceState::Connected;
        log->debug("[%s: %s] Neuro device connected. Device %s, address: %s", "ParameterReader", __FUNCTION__,
                   mBleDevice->getName().c_str(), mBleDevice->getNetAddress().c_str());
    }
    else{
        log->error("[%s: %s] Neuro device protocol error. Device %s, address: %s", "ParameterReader", __FUNCTION__,
                   mBleDevice->getName().c_str(), mBleDevice->getNetAddress().c_str());
        mBleDevice->disconnect();
    }
}

void ParameterReader::onBleDisconnected(BleDeviceError error){
    auto log = LoggerFactory::getCurrentPlatformLogger();
    mState = DeviceState::Disconnected;
    if (error != BleDeviceError::NO_ERROR)        {
        log->error("[%s: %s] Neuro device bluetooth error. Device %s, address: %s", "ParameterReader", __FUNCTION__,
                   mBleDevice->getName().c_str(), mBleDevice->getNetAddress().c_str());
    }
    else{
        log->debug("[%s: %s] Ble device disconnected. Device %s, address: %s", "ParameterReader", __FUNCTION__,
                   mBleDevice->getName().c_str(), mBleDevice->getNetAddress().c_str());
    }
}

}
