#include "ble/win/ble_device_win.h"
#include "logger.h"
#include <sstream>
#include <iomanip>

namespace Neuro {

namespace {
constexpr const char *class_name = "BleDeviceWin";
}

void rxCharacteristicValueChanged(BTH_LE_GATT_EVENT_TYPE, PVOID eventOutParameter, PVOID context){
    auto bleDevice = reinterpret_cast<BleDeviceWin *>(context);
    auto valueChangedParams = reinterpret_cast<PBLUETOOTH_GATT_VALUE_CHANGED_EVENT>(eventOutParameter);
    auto valueDataPtr = valueChangedParams->CharacteristicValue->Data;
    auto valueDataSize = valueChangedParams->CharacteristicValue->DataSize;
    LOG_TRACE_V("Rx data size is %d", valueDataSize);
    ByteBuffer data(valueDataPtr, valueDataPtr + valueDataSize);
    if (bleDevice->dataReceivedCallback)
        bleDevice->dataReceivedCallback(data);
}

void statusCharacteristicValueChanged(BTH_LE_GATT_EVENT_TYPE, PVOID eventOutParameter, PVOID context){
    auto bleDevice = reinterpret_cast<BleDeviceWin *>(context);
    auto valueChangedParams = reinterpret_cast<PBLUETOOTH_GATT_VALUE_CHANGED_EVENT>(eventOutParameter);
    auto valueDataPtr = valueChangedParams->CharacteristicValue->Data;
    auto valueDataSize = valueChangedParams->CharacteristicValue->DataSize;
    ByteBuffer data(valueDataPtr, valueDataPtr + valueDataSize);
    if (bleDevice->statusReceivedCallback)
        bleDevice->statusReceivedCallback(data);
}

BleDeviceWin::BleDeviceWin(DeviceHandle &&device_handle, std::string name, std::string address) :
    BleDevice(BleDeviceInfo::fromDeviceName(name)),
    mServiceHandle(std::move(device_handle)),
    mName(name),
    mAddress(address),
    mService(get_service(mServiceHandle, mDeviceInfo->getGattInfo()->deviceServiceUUID())),
    mRxCharacteristic(get_device_characteristic(mServiceHandle, mService, mDeviceInfo->getGattInfo()->rxCharacteristicUUID())),
    mTxCharacteristic(get_device_characteristic(mServiceHandle, mService, mDeviceInfo->getGattInfo()->txCharacteristicUUID())),
    mHasStatusCharacteristic(mDeviceInfo->getGattInfo()->statusCharacteristicUUID() != std::string("")),
    mTaskSpawnLoop(&BleDeviceWin::spawnGetDescriptorsTask, std::chrono::seconds(1), this){
    if (mHasStatusCharacteristic){
        mStatusCharacteristic = get_device_characteristic(mServiceHandle, mService, mDeviceInfo->getGattInfo()->statusCharacteristicUUID());
    }
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
    return write_characteristic(mServiceHandle, mTxCharacteristic, commandData);
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

        mCCCDDescriptor = get_descriptor(mServiceHandle, mRxCharacteristic, mDeviceInfo->getGattInfo()->cccd());
        while(true){
            try {
                get_descriptor_value(mServiceHandle, mCCCDDescriptor);
                break;
            }
            catch (gatt_timeout &){
                mIsConnected.store(false);
                LOG_TRACE_V("Device %s [%s] is offline. Trying to reconnect", mName.c_str(), mAddress.c_str());
            }
            catch (std::runtime_error &e){
                mIsConnected.store(false);                
                LOG_WARN_V("Error read descriptor for device %s [%s]: %s", mName.c_str(), mAddress.c_str(), e.what());
                return;
            }
        }
        BTH_LE_GATT_DESCRIPTOR_VALUE newValue;
        RtlZeroMemory(&newValue, sizeof(newValue));
        newValue.DescriptorType = ClientCharacteristicConfiguration;
        newValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;
        if (set_descriptor_value(mServiceHandle, mCCCDDescriptor, newValue)){
            subscribe_characteristic_value_changed(mServiceHandle,
                                                   mRxCharacteristic,
                                                   reinterpret_cast<PFNBLUETOOTH_GATT_EVENT_CALLBACK>(&rxCharacteristicValueChanged),
                                                   this);
        }
        else {
            mIsConnected.store(false);
            LOG_ERROR_V("Unable set notifications for device %s [%s]", mName.c_str(), mAddress.c_str());
        }

        if (mHasStatusCharacteristic){
            auto statusCccd = get_descriptor(mServiceHandle, mStatusCharacteristic, mDeviceInfo->getGattInfo()->cccd());
            try {
                get_descriptor_value(mServiceHandle, statusCccd);
            }
            catch (gatt_timeout &){
                mIsConnected.store(false);
                LOG_TRACE_V("Device %s [%s] is offline. Trying to reconnect", mName.c_str(), mAddress.c_str());
                return;
            }
            catch (std::runtime_error &e){
                mIsConnected.store(false);
                LOG_WARN_V("Error read descriptor for device %s [%s]: %s", mName.c_str(), mAddress.c_str(), e.what());
                return;
            }
            BTH_LE_GATT_DESCRIPTOR_VALUE newStatusValue;
            RtlZeroMemory(&newStatusValue, sizeof(newStatusValue));
            newStatusValue.DescriptorType = ClientCharacteristicConfiguration;
            newStatusValue.ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;
            if (set_descriptor_value(mServiceHandle, statusCccd, newStatusValue)){
                subscribe_characteristic_value_changed(mServiceHandle,
                                                       mStatusCharacteristic,
                                                       reinterpret_cast<PFNBLUETOOTH_GATT_EVENT_CALLBACK>(&statusCharacteristicValueChanged),
                                                       this);
            }
            else {
                mIsConnected.store(false);
                LOG_ERROR_V("Unable set status notifications for device %s [%s]", mName.c_str(), mAddress.c_str());
                return;
            }
        }

        mIsConnected.store(true);
        if (deviceStateChangedCallback)
            deviceStateChangedCallback(BleDeviceState::Connected, BleDeviceError::NoError);
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
                get_descriptor_value(mServiceHandle, mCCCDDescriptor);
            }
            catch (std::runtime_error &){
                LOG_WARN_V("Device %s [%s] is offline. State disconnected", mName.c_str(), mAddress.c_str());
                performDisconnect();
            }
        }
    });
}

}
