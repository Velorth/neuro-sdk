#include "gsl/gsl_assert"
#include "channels/info/channel_notification.h"
#include "device/device_impl.h"

namespace Neuro {

template<>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Battery>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Battery>(std::function<void (const typename ChannelData<ChannelInfo::Type::Battery>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeBatteryDataReceived(callback);
}

template<>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Signal>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Signal>(std::function<void (const typename ChannelData<ChannelInfo::Type::Signal>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeSignalDataReceived(callback);
}

template<>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Resistance>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(std::function<void (const typename ChannelData<ChannelInfo::Type::Resistance>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeResistanceDataReceived(callback);
}

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::MEMS>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(std::function<void(const typename ChannelData<ChannelInfo::Type::MEMS>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeMEMSDataReceived(callback);
}
template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Orientation>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(std::function<void(const typename ChannelData<ChannelInfo::Type::Orientation>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeOrientationDataReceived(callback);
}

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::ElectrodesState>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(std::function<void(const typename ChannelData<ChannelInfo::Type::ElectrodesState>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeElectrodesDataReceived(callback);
}

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Respiration>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(std::function<void(const typename ChannelData<ChannelInfo::Type::Respiration>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeRespirationDataReceived(callback);
}

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::ConnectionStats>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(std::function<void(const typename ChannelData<ChannelInfo::Type::ConnectionStats>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribeConnectionStatsDataReceived(callback);
}

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Pedometer>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(std::function<void(const typename ChannelData<ChannelInfo::Type::Pedometer>::Type &)> callback){
    Expects(mImpl != nullptr);
    return mImpl->subscribePedometerDataReceived(callback);
}

}
