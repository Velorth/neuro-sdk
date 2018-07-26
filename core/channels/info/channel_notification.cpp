#include "gsl/gsl_assert"
#include "channels/info/channel_notification.h"
#include "device/device_impl.h"

namespace Neuro {

template<>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Battery> &>
Device::subscribeDataReceived<ChannelInfo::Type::Battery>(std::function<void (const channel_data_t<ChannelInfo::Type::Battery> &)> callback,
                                                          ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeBatteryDataReceived(callback, info);
}

template<>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Signal> &>
Device::subscribeDataReceived<ChannelInfo::Type::Signal>(std::function<void (const channel_data_t<ChannelInfo::Type::Signal> &)> callback,
                                                         ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeSignalDataReceived(callback, info);
}

template<>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Resistance> &>
Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(std::function<void (const channel_data_t<ChannelInfo::Type::Resistance> &)> callback,
                                                             ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeResistanceDataReceived(callback, info);
}

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::MEMS> &>
Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(std::function<void(const channel_data_t<ChannelInfo::Type::MEMS> &)> callback,
                                                       ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeMEMSDataReceived(callback, info);
}
template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Orientation> &>
Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(std::function<void(const channel_data_t<ChannelInfo::Type::Orientation> &)> callback,
                                                              ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeOrientationDataReceived(callback, info);
}

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::ElectrodesState> &>
Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(std::function<void(const channel_data_t<ChannelInfo::Type::ElectrodesState> &)> callback,
                                                                  ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeElectrodesDataReceived(callback, info);
}

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Respiration> &>
Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(std::function<void(const channel_data_t<ChannelInfo::Type::Respiration> &)> callback,
                                                              ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeRespirationDataReceived(callback, info);
}

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::ConnectionStats> &>
Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(std::function<void(const channel_data_t<ChannelInfo::Type::ConnectionStats> &)> callback,
                                                                  ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribeConnectionStatsDataReceived(callback, info);
}

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Pedometer> &>
Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(std::function<void(const channel_data_t<ChannelInfo::Type::Pedometer> &)> callback,
                                                            ChannelInfo info){
    Expects(mImpl != nullptr);
    return mImpl->subscribePedometerDataReceived(callback, info);
}

}
