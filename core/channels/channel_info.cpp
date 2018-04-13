#include "channels/channel_info.h"

namespace Neuro {

const ChannelInfo ChannelInfo::Signal = ChannelInfo(Type::Signal);
const ChannelInfo ChannelInfo::Battery = ChannelInfo(Type::Battery);
const ChannelInfo ChannelInfo::ElectrodesState = ChannelInfo(Type::ElectrodesState);
const ChannelInfo ChannelInfo::Respiration = ChannelInfo(Type::Respiration);
const ChannelInfo ChannelInfo::MEMS = ChannelInfo(Type::MEMS);
const ChannelInfo ChannelInfo::Angle = ChannelInfo(Type::Angle);
const ChannelInfo ChannelInfo::ConnectionStats = ChannelInfo(Type::ConnectionStats);
const ChannelInfo ChannelInfo::Resistance = ChannelInfo(Type::Resistance);
const ChannelInfo ChannelInfo::Pedometer = ChannelInfo(Type::Pedometer);

ChannelInfo::ChannelInfo(ChannelInfo::Type type):
    mType(type),
    mName(typeToName(mType)){
}

ChannelInfo::ChannelInfo(Type type, std::string &&name) noexcept :
    mType(type),
    mName(std::move(name)){
}

ChannelInfo::ChannelInfo(Type type, const std::string &name):
    mType(type),
    mName(name){
}

std::string ChannelInfo::getName() const {
    return mName;
}

void ChannelInfo::setName(std::string &&name) noexcept {
    mName = std::move(name);
}

void ChannelInfo::setName(const std::string &name) {
    mName = name;
}

ChannelInfo::Type ChannelInfo::getType() const noexcept {
    return mType;
}

bool ChannelInfo::operator==(const ChannelInfo &rhs) const noexcept {
    return (mType == rhs.mType) && (mName == rhs.mName);
}

bool ChannelInfo::operator!=(const ChannelInfo &rhs) const noexcept {
    return !(*this == rhs);
}

}
