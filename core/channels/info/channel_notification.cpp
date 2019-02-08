#include "channels/info/channel_notification.h"
#include "device/device_impl.h"

namespace Neuro {

template<>
ChannelDataListenerType<ChannelInfo::Type::Battery>
Device::subscribeDataReceived<ChannelInfo::Type::Battery>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Battery> callback, ChannelInfo info) {
    return mImpl->subscribeBatteryDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::Signal>
Device::subscribeDataReceived<ChannelInfo::Type::Signal>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Signal> callback, ChannelInfo info) {
    return mImpl->subscribeSignalDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::Resistance>
Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Resistance> callback, ChannelInfo info) {
    return mImpl->subscribeResistanceDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::MEMS>
Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(ChannelDataCallbackFunctionType<ChannelInfo::Type::MEMS> callback, ChannelInfo info) {
    return mImpl->subscribeMEMSDataReceived(callback, info);
}
template <>
ChannelDataListenerType<ChannelInfo::Type::Orientation>
Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Orientation> callback, ChannelInfo info) {
    return mImpl->subscribeOrientationDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::ElectrodesState>
Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(ChannelDataCallbackFunctionType<ChannelInfo::Type::ElectrodesState> callback, ChannelInfo info) {
    return mImpl->subscribeElectrodesDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::Respiration>
Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Respiration> callback, ChannelInfo info) {
    return mImpl->subscribeRespirationDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::ConnectionStats>
Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(ChannelDataCallbackFunctionType<ChannelInfo::Type::ConnectionStats> callback, ChannelInfo info) {
    return mImpl->subscribeConnectionStatsDataReceived(callback, info);
}

template <>
ChannelDataListenerType<ChannelInfo::Type::Pedometer>
Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Pedometer> callback, ChannelInfo info) {
    return mImpl->subscribePedometerDataReceived(callback, info);
}

}
