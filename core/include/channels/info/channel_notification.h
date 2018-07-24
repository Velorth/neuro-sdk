#ifndef CHANNEL_NOTIFICATION_H
#define CHANNEL_NOTIFICATION_H

#include "device/device.h"

namespace Neuro {

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Battery>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Battery>(std::function<void(const ChannelData<ChannelInfo::Type::Battery>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Signal>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Signal>(std::function<void(const ChannelData<ChannelInfo::Type::Signal>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Resistance>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(std::function<void(const ChannelData<ChannelInfo::Type::Resistance>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::MEMS>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(std::function<void(const ChannelData<ChannelInfo::Type::MEMS>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Orientation>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(std::function<void(const ChannelData<ChannelInfo::Type::Orientation>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::ElectrodesState>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(std::function<void(const ChannelData<ChannelInfo::Type::ElectrodesState>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Respiration>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(std::function<void(const ChannelData<ChannelInfo::Type::Respiration>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::ConnectionStats>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(std::function<void(const ChannelData<ChannelInfo::Type::ConnectionStats>::Type &)>);

template <>
ListenerPtr<void, const ChannelData<ChannelInfo::Type::Pedometer>::Type &>
Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(std::function<void(const ChannelData<ChannelInfo::Type::Pedometer>::Type &)>);

}

#endif // CHANNEL_NOTIFICATION_H
