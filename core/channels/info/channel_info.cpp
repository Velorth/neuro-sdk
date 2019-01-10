#include "channels/info/channel_info.h"

namespace Neuro {

ChannelInfo ChannelInfo::Signal(){
    static ChannelInfo SignalInfo = ChannelInfo(Type::Signal);
    return SignalInfo;
}

ChannelInfo ChannelInfo::Battery(){
    static ChannelInfo BatteryInfo = ChannelInfo(Type::Battery);
    return BatteryInfo;
}

ChannelInfo ChannelInfo::ElectrodesState(){
    static ChannelInfo ElectrodesStateInfo = ChannelInfo(Type::ElectrodesState);
    return ElectrodesStateInfo;
}

ChannelInfo ChannelInfo::Respiration(){
    static ChannelInfo RespirationInfo = ChannelInfo(Type::Respiration);
    return RespirationInfo;
}

ChannelInfo ChannelInfo::MEMS(){
    static ChannelInfo MEMSInfo = ChannelInfo(Type::MEMS);
    return MEMSInfo;
}

ChannelInfo ChannelInfo::Orientation(){
    static ChannelInfo OrientationInfo = ChannelInfo(Type::Orientation);
    return OrientationInfo;
}

ChannelInfo ChannelInfo::ConnectionStats(){
    static ChannelInfo ConnectionStatsInfo = ChannelInfo(Type::ConnectionStats);
    return ConnectionStatsInfo;
}

ChannelInfo ChannelInfo::Resistance(){
    static ChannelInfo ResistanceInfo = ChannelInfo(Type::Resistance);
    return ResistanceInfo;
}

ChannelInfo ChannelInfo::Pedometer(){
    static ChannelInfo PedometerInfo = ChannelInfo(Type::Pedometer);
    return PedometerInfo;
}

ChannelInfo::ChannelInfo(ChannelInfo::Type type):
    mType(type),
    mName(typeToName(mType)),
    mIndex(0){
}

ChannelInfo::ChannelInfo(Type type, std::string &&name, std::size_t index) noexcept :
    mType(type),
    mName(std::move(name)),
    mIndex(index){
}

ChannelInfo::ChannelInfo(Type type, const std::string &name, std::size_t index):
    mType(type),
    mName(name),
    mIndex(index){
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

std::size_t ChannelInfo::getIndex() const noexcept{
    return mIndex;
}

void ChannelInfo::swap(ChannelInfo& rhs) noexcept {
	using std::swap;
	swap(mType, rhs.mType);
	swap(mName, rhs.mName);
	swap(mIndex, rhs.mIndex);
}

void swap(ChannelInfo &lhs, ChannelInfo &rhs) noexcept {
	lhs.swap(rhs);
}

bool operator==(const ChannelInfo &lhs, const ChannelInfo &rhs){
    return (lhs.getType() == rhs.getType()) && (lhs.getName() == rhs.getName()) && (lhs.getIndex() == rhs.getIndex());
}

bool operator!=(const ChannelInfo &lhs, const ChannelInfo &rhs){
    return !(lhs == rhs);
}

}
