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
    const Type mType;
    std::string mName;
    const std::size_t mIndex;
};

template <ChannelInfo::Type DataType>
struct ChannelData;

template <>
struct ChannelData<ChannelInfo::Type::Signal>{
    using Type = std::vector<signal_sample_t>;
};

template <>
struct ChannelData<ChannelInfo::Type::Battery>{
    using Type = std::vector<int>;
};

template <>
struct ChannelData<ChannelInfo::Type::Resistance>{
    using Type = std::vector<resistance_sample_t>;
};

template <>
struct ChannelData<ChannelInfo::Type::Respiration>{
    using Type = std::vector<double>;
};

template <>
struct ChannelData<ChannelInfo::Type::MEMS>{
    using Type = std::vector<MEMS>;
};

template <>
struct ChannelData<ChannelInfo::Type::Orientation>{
    using Type = std::vector<Quaternion>;
};

template <>
struct ChannelData<ChannelInfo::Type::ConnectionStats>{
    using Type = std::vector<int>;
};

template <>
struct ChannelData<ChannelInfo::Type::ElectrodesState>{
    using Type = std::vector<ElectrodeState>;
};

template <>
struct ChannelData<ChannelInfo::Type::Pedometer>{
    using Type = std::vector<int>;
};

}

#endif // CHANNEL_INFO_H
