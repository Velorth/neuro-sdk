#ifndef CHANNEL_INFO_H
#define CHANNEL_INFO_H

#include <string>
#include "common_types.h"
#include "mems_data.h"
#include "quaternion.h"
#include "electrode_state.h"
#include "lib_export.h"

namespace Neuro {

class SDK_SHARED ChannelInfo final {
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

    static ChannelInfo Signal();
    static ChannelInfo Battery();
    static ChannelInfo ElectrodesState();
    static ChannelInfo Respiration();
    static ChannelInfo MEMS();
    static ChannelInfo Orientation();
    static ChannelInfo ConnectionStats();
    static ChannelInfo Resistance();
    static ChannelInfo Pedometer();

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
    Type mType;
    std::string mName;
    std::size_t mIndex;
};

template <ChannelInfo::Type InfoType>
struct ChannelTraits;

template <>
struct ChannelTraits<ChannelInfo::Type::Signal>{
    using DataType = std::vector<signal_sample_t>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Signal(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::Battery>{
    using DataType = std::vector<int>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Battery(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::Resistance>{
    using DataType = std::vector<resistance_sample_t>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Resistance(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::Respiration>{
    using DataType = std::vector<double>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Respiration(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::MEMS>{
    using DataType = std::vector<MEMS>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::MEMS(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::Orientation>{
    using DataType = std::vector<Quaternion>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Orientation(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::ConnectionStats>{
    using DataType = std::vector<int>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::ConnectionStats(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::ElectrodesState>{
    using DataType = std::vector<ElectrodeState>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::ElectrodesState(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::Pedometer>{
    using DataType = std::vector<int>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Pedometer(); }
};

template <ChannelInfo::Type InfoType>
using channel_data_t = typename ChannelTraits<InfoType>::DataType;

template <ChannelInfo::Type InfoType>
ChannelInfo default_channel_info(){
    return ChannelTraits<InfoType>::defaultInfo();
}

}

#endif // CHANNEL_INFO_H
