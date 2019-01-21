#include "gsl/gsl_assert"
#include "device/parameter_reader.h"
#include "ble/ble_device.h"
#include "logger.h"

namespace Neuro {

ParameterReader::ParameterReader(std::shared_ptr<BleDevice> ble_device,
                                 param_changed_callback_t callback):
    mBleDevice(ble_device),
    parameterChangedCallback(callback){
    Ensures(mBleDevice != nullptr);
    Ensures(parameterChangedCallback != nullptr);
    subscribeBleDeviceStateChanged();
}

void ParameterReader::requestConnect(){
    Expects(mBleDevice != nullptr);
    mBleDevice->connect();
}

void ParameterReader::requestDisconnect(){
    Expects(mBleDevice != nullptr);
    mPendingDisconnectRequest = true;
    mBleDevice->disconnect();
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
   mBleStateListener = mBleDevice->subscribeConnectionStateChanged([=](BleDeviceState state, BleDeviceError error){
       onBleDeviceStateChanged(state, error);
   });
}

void ParameterReader::onBleDeviceStateChanged(BleDeviceState state, BleDeviceError error) {
    if (state == BleDeviceState::Connected) {
        onBleConnected();
    }
    else {
        onBleDisconnected(error);
    }
}

void ParameterReader::onBleConnected(){
    LOG_DEBUG_V("Ble device connected. Device %s, address: %s. Initializing...",
                mBleDevice->getName().c_str(),
                mBleDevice->getNetAddress().c_str());
    if (loadDeviceParams()) {        
        LOG_DEBUG_V("Neuro device connected. Device %s, address: %s",
                    mBleDevice->getName().c_str(),
                    mBleDevice->getNetAddress().c_str());
        mState = DeviceState::Connected;
        parameterChangedCallback(Parameter::State);
    }
    else{
        LOG_ERROR_V("Neuro device protocol error. Device %s, address: %s",
                    mBleDevice->getName().c_str(),
                    mBleDevice->getNetAddress().c_str());
        mBleDevice->disconnect();
    }
}

void ParameterReader::onBleDisconnected(BleDeviceError error){
    mState = DeviceState::Disconnected;
    parameterChangedCallback(Parameter::State);
    if (error != BleDeviceError::NoError){
        LOG_ERROR_V("Neuro device bluetooth error. Device %s, address: %s",
                    mBleDevice->getName().c_str(),
                    mBleDevice->getNetAddress().c_str());
    }
    else{
        LOG_DEBUG_V("Ble device disconnected. Device %s, address: %s",
                    mBleDevice->getName().c_str(),
                    mBleDevice->getNetAddress().c_str());
        if (!mPendingDisconnectRequest){
            LOG_DEBUG("No pending disconnect requests registered. Reconnecting...");
            mBleDevice->connect();
        }
        else {
            LOG_DEBUG("Pending disconnect requests registered. Device disconnected.");
            mPendingDisconnectRequest = false;
        }
    }
}

}
