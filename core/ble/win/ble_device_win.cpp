#include "ble/win/ble_device_win.h"
#include "logger.h"

namespace Neuro {

BleDeviceWin::BleDeviceWin(DeviceHandle &&device_handle, std::string name, std::string address) :
    BleDevice(BleDeviceInfo::fromDeviceName(name)),
    mDeviceHandle(std::move(device_handle)),
    mName(name),
    mAddress(address),
    mService(get_service(mDeviceHandle, mDeviceInfo->getGattInfo()->deviceServiceUUID())),
    mRxCharacteristic(get_device_characteristic(mDeviceHandle, mService, mDeviceInfo->getGattInfo()->rxCharacteristicUUID())),
    mTxCharacteristic(get_device_characteristic(mDeviceHandle, mService, mDeviceInfo->getGattInfo()->txCharacteristicUUID())),
    mTaskSpawnLoop(&BleDeviceWin::spawnGetDescriptorsTask, std::chrono::seconds(1), this){

}

void BleDeviceWin::connect(){
    if(mIsConnected.load()){
        return;
    }
    performConnect();
}

void BleDeviceWin::disconnect(){
    if (!mIsConnected.load()){
        return;
    }
    performDisconnect();
}

void BleDeviceWin::close(){
    disconnect();
}

bool BleDeviceWin::sendCommand(const std::vector<Byte> &commandData){
    if (!mIsConnected.load()){
        return false;
    }
    return write_characteristic(mDeviceHandle, mTxCharacteristic, commandData);
}

BleDeviceState BleDeviceWin::getState() const {
    return mIsConnected.load() ? BleDeviceState::Connected : BleDeviceState::Disconnected;
}

std::string BleDeviceWin::getName() const {
    return mName;
}

std::string BleDeviceWin::getNetAddress() const {
    return mAddress;
}

void BleDeviceWin::performConnect(){
    mDeviceTaskQueue.exec([=](){
        if (mIsConnected.load())
            return;

        mCCCDDescriptor = get_descriptor(mDeviceHandle, mRxCharacteristic, mDeviceInfo->getGattInfo()->cccd());
        while(true){
            try {
                get_descriptor_value(mDeviceHandle, mCCCDDescriptor);
                break;
            }
            catch (...){
                mIsConnected.store(false);
                LOG_WARN_V("Device %s [%s] is offline. Trying to reconnect", mName.c_str(), mAddress.c_str());
            }
        }
        BTH_LE_GATT_DESCRIPTOR_VALUE newValue;
        newValue.DescriptorType = ClientCharacteristicConfiguration;
        newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;
        if (set_descriptor_value(mDeviceHandle, mCCCDDescriptor, newValue)){
            subscribe_characteristic_value_changed(mDeviceHandle, mTxCharacteristic, [=](BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context){});
        }
        else {
            mIsConnected.store(false);
            LOG_ERROR_V("Unable set notifications for device %s [%s]", mName.c_str(), mAddress.c_str());
        }
    });
}

void BleDeviceWin::performDisconnect(){
    mDeviceTaskQueue.exec([=](){
        mIsConnected.store(false);
        if (deviceStateChangedCallback)
            deviceStateChangedCallback(BleDeviceState::Disconnected, BleDeviceError::NoError);
    });
}

void BleDeviceWin::spawnGetDescriptorsTask(){
    mDeviceTaskQueue.exec([=](){
        if (mIsConnected.load()){
            try {
                get_descriptor_value(mDeviceHandle, mCCCDDescriptor);
            }
            catch (std::runtime_error &){
                LOG_WARN_V("Device %s [%s] is offline. State disconnected", mName.c_str(), mAddress.c_str());
                performDisconnect();
            }
        }
    });
}

}
