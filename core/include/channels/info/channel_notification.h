#ifndef CHANNEL_NOTIFICATION_H
#define CHANNEL_NOTIFICATION_H

#include "device/device.h"

namespace Neuro {

template <>
ChannelDataListenerType<ChannelInfo::Type::Battery>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::Battery>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Battery>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Signal>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::Signal>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Signal>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Resistance>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::Resistance>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Resistance>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::MEMS>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::MEMS>(ChannelDataCallbackFunctionType<ChannelInfo::Type::MEMS>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Orientation>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::Orientation>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Orientation>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::ElectrodesState>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::ElectrodesState>(ChannelDataCallbackFunctionType<ChannelInfo::Type::ElectrodesState>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Respiration>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::Respiration>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Respiration>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::ConnectionStats>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::ConnectionStats>(ChannelDataCallbackFunctionType<ChannelInfo::Type::ConnectionStats>, ChannelInfo);

template <>
ChannelDataListenerType<ChannelInfo::Type::Pedometer>
SDK_SHARED Device::subscribeDataReceived<ChannelInfo::Type::Pedometer>(ChannelDataCallbackFunctionType<ChannelInfo::Type::Pedometer>, ChannelInfo);

}

#endif // CHANNEL_NOTIFICATION_H
