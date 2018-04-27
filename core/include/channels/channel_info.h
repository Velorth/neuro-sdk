#ifndef CHANNEL_INFO_H
#define CHANNEL_INFO_H

#include <string>

namespace Neuro {

class ChannelInfo final {
public:
    enum class Type {
        Signal,
        Battery,
        ElectrodesState,
        Respiration,
        MEMS,
        Orientation,
        ConnectionStats,
        Resistance,
        Pedometer,
        Custom
    };

    static inline std::string typeToName(Type type) {
        switch(type){
        case Type::Signal: return "Signal";
        case Type::Battery: return "Battery";
        case Type::ElectrodesState: return "ElectrodesState";
        case Type::Respiration: return "Respiration";
        case Type::MEMS: return "MEMS";
        case Type::Orientation: return "Orientation";
        case Type::ConnectionStats: return "ConnectionStats";
        case Type::Resistance: return "Resistance";
        case Type::Pedometer: return "Pedometer";
        case Type::Custom: default: return "Custom";
        }
    }

    static const ChannelInfo Signal;
    static const ChannelInfo Battery;
    static const ChannelInfo ElectrodesState;
    static const ChannelInfo Respiration;
    static const ChannelInfo MEMS;
    static const ChannelInfo Orientation;
    static const ChannelInfo ConnectionStats;
    static const ChannelInfo Resistance;
    static const ChannelInfo Pedometer;

    ChannelInfo(Type);
    ChannelInfo(Type, std::string &&, std::size_t = 0) noexcept;
    ChannelInfo(Type, const std::string &, std::size_t = 0);
    ChannelInfo(const ChannelInfo &) = default;
    ChannelInfo& operator=(const ChannelInfo &) = default;
    ChannelInfo(ChannelInfo &&) = default;
    ChannelInfo& operator=(ChannelInfo &&) = default;

    std::string getName() const;
    void setName(std::string &&) noexcept;
    void setName(const std::string &);
    Type getType() const noexcept;
    std::size_t getIndex() const noexcept;

    bool operator==(const ChannelInfo &) const noexcept;
    bool operator!=(const ChannelInfo &) const noexcept;

private:
    const Type mType;
    std::string mName;
    const std::size_t mIndex;
};

}

#endif // CHANNEL_INFO_H
