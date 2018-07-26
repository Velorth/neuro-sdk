#ifndef CHANNEL_NOTIFICATION_H
#define CHANNEL_NOTIFICATION_H

#include "device/device.h"

namespace Neuro {

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Battery> &>
Device::subscribeDataReceived<ChannelInfo::Type::Battery>(std::function<void(const channel_data_t<ChannelInfo::Type::Battery> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Signal> &>
Device::subscribeDataReceived<ChannelInfo::Type::Signal>(std::function<void(const channel_data_t<ChannelInfo::Type::Signal> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Resistance> &>
Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(std::function<void(const channel_data_t<ChannelInfo::Type::Resistance> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::MEMS> &>
Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(std::function<void(const channel_data_t<ChannelInfo::Type::MEMS> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Orientation> &>
Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(std::function<void(const channel_data_t<ChannelInfo::Type::Orientation> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::ElectrodesState> &>
Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(std::function<void(const channel_data_t<ChannelInfo::Type::ElectrodesState> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Respiration> &>
Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(std::function<void(const channel_data_t<ChannelInfo::Type::Respiration> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::ConnectionStats> &>
Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(std::function<void(const channel_data_t<ChannelInfo::Type::ConnectionStats> &)>, ChannelInfo);

template <>
ListenerPtr<void, const channel_data_t<ChannelInfo::Type::Pedometer> &>
Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(std::function<void(const channel_data_t<ChannelInfo::Type::Pedometer> &)>, ChannelInfo);

}

#endif // CHANNEL_NOTIFICATION_H
