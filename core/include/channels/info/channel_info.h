#ifndef CHANNEL_INFO_H
#define CHANNEL_INFO_H

#include <string>
#include <functional>
#include <vector>
#include "common_types.h"
#include "mems_data.h"
#include "quaternion.h"
#include "electrode_state.h"
#include "lib_export.h"
#include "event_listener.h"

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
	~ChannelInfo() = default;

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
    using DataType = signal_sample_t;
	using CallbackFunctionType = std::function<void(const std::vector<DataType> &)>;
	using DataListenerType = ListenerPtr<void, const std::vector<DataType> &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Signal(); }
	static constexpr std::size_t MemoryBufferSize = 150000; //10 minutes for 250 Hz sampling frequency;
};

template <>
struct ChannelTraits<ChannelInfo::Type::Resistance>{
    using DataType = resistance_sample_t;
	using CallbackFunctionType = std::function<void(const std::vector<DataType> &)>;
	using DataListenerType = ListenerPtr<void, const std::vector<DataType> &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Resistance(); }
	static constexpr std::size_t MemoryBufferSize = 150000;
};

template <>
struct ChannelTraits<ChannelInfo::Type::Respiration>{
    using DataType = double;
	using CallbackFunctionType = std::function<void(const std::vector<DataType> &)>;
	using DataListenerType = ListenerPtr<void, const std::vector<DataType> &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Respiration(); }
	static constexpr float SamplingFrequency = 100.0f;
	static constexpr std::size_t MemoryBufferSize = 60000;
};

template <>
struct ChannelTraits<ChannelInfo::Type::MEMS>{
    using DataType = MEMS;
	using CallbackFunctionType = std::function<void(const std::vector<DataType> &)>;
	using DataListenerType = ListenerPtr<void, const std::vector<DataType> &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::MEMS(); }
	static constexpr float SamplingFrequency = 100.0f;
	static constexpr std::size_t MemoryBufferSize = 60000;
};

template <>
struct ChannelTraits<ChannelInfo::Type::Orientation>{
    using DataType = Quaternion;
	using CallbackFunctionType = std::function<void(const std::vector<DataType> &)>;
	using DataListenerType = ListenerPtr<void, const std::vector<DataType> &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::Orientation(); }
	static constexpr float SamplingFrequency = 100.0f;
	static constexpr std::size_t MemoryBufferSize = 60000;
};

template <>
struct ChannelTraits<ChannelInfo::Type::Pedometer> {
	using DataType = int;
	using CallbackFunctionType = std::function<void(const std::vector<DataType> &)>;
	using DataListenerType = ListenerPtr<void, const std::vector<DataType> &>;
	static ChannelInfo defaultInfo() { return ChannelInfo::Pedometer(); }
	static constexpr float SamplingFrequency = 100.0f;
	static constexpr std::size_t MemoryBufferSize = 60000;
};

template <>
struct ChannelTraits<ChannelInfo::Type::Battery> {
	using DataType = int;
	using CallbackFunctionType = std::function<void(const DataType &)>;
	using DataListenerType = ListenerPtr<void, const DataType &>;
	static ChannelInfo defaultInfo() { return ChannelInfo::Battery(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::ConnectionStats>{
    using DataType = int;
	using CallbackFunctionType = std::function<void(const DataType &)>;
	using DataListenerType = ListenerPtr<void, const DataType &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::ConnectionStats(); }
};

template <>
struct ChannelTraits<ChannelInfo::Type::ElectrodesState>{
    using DataType = ElectrodeState;
	using CallbackFunctionType = std::function<void(const DataType &)>;
	using DataListenerType = ListenerPtr<void, const DataType &>;
    static ChannelInfo defaultInfo(){ return ChannelInfo::ElectrodesState(); }
};


template <ChannelInfo::Type InfoType>
using ChannelDataType = typename ChannelTraits<InfoType>::DataType;

template <ChannelInfo::Type InfoType>
using ChannelDataCallbackFunctionType = typename ChannelTraits<InfoType>::CallbackFunctionType;

template <ChannelInfo::Type InfoType>
using ChannelDataListenerType = typename ChannelTraits<InfoType>::DataListenerType;

template <ChannelInfo::Type InfoType>
ChannelInfo default_channel_info(){
    return ChannelTraits<InfoType>::defaultInfo();
}

}

#endif // CHANNEL_INFO_H
