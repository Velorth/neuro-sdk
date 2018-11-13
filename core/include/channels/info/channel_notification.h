#ifndef CHANNEL_NOTIFICATION_H
#define CHANNEL_NOTIFICATION_H

#include "device/device.h"

namespace Neuro {

template <>
ChannelDataListenerType<ChannelInfo::Type::Battery>
Device::subscribeDataReceived<ChannelInfo::Type::Battery>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Battery>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Signal>
Device::subscribeDataReceived<ChannelInfo::Type::Signal>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Signal>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Resistance>
Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Resistance>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::MEMS>
Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(ChannelDataCallbackFunctionType<ChannelInfo::Type::MEMS>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Orientation>
Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Orientation>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::ElectrodesState>
Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(ChannelDataCallbackFunctionType<ChannelInfo::Type::ElectrodesState>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Respiration>
Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Respiration>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::ConnectionStats>
Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(ChannelDataCallbackFunctionType<ChannelInfo::Type::ConnectionStats>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Pedometer>
Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Pedometer>, ChannelInfo);

}

#endif // CHANNEL_NOTIFICATION_H
