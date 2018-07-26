#ifndef DEVICE_H
#define DEVICE_H

#include <memory>
#include <vector>
#include <functional>
#include "channels/info/channel_info.h"
#include "device_parameters.h"
#include "event_listener.h"
#include "lib_export.h"

namespace Neuro {

class DeviceImpl;

class SDK_SHARED Device final {
private:
    std::unique_ptr<DeviceImpl> mImpl;

public:
    ~Device();

    void connect();
    void disconnect();
    std::vector<ChannelInfo> channels() const;
    std::vector<Command> commands() const;
    std::vector<ParamPair> parameters() const;
    bool execute(Command);
    void setParamChangedCallback(std::function<void(Parameter)>);

    template <Parameter P>
    typename ParamValue<P>::Type readParam() const;

    template <Parameter P>
    bool setParam(typename ParamValue<P>::Type value);

    template <ChannelInfo::Type Channel>
    ListenerPtr<void, const channel_data_t<Channel> &>
    subscribeDataReceived(std::function<void(const channel_data_t<Channel> &)>,
                          ChannelInfo info = default_channel_info<Channel>());

private:
    friend class DeviceFactory;
    friend class BatteryChannel;
    friend class ElectrodeStateChannel;
    friend class SignalChannel;
    friend class RespirationChannel;
    friend class MemsChannel;
    friend class OrientationChannel;
    friend class ConnectionStatsChannel;
    Device(std::unique_ptr<DeviceImpl>);
};

void libDeviceDeleter(Device *);
using DeviceDeleter = decltype(&libDeviceDeleter);

using DeviceUniquePtr = std::unique_ptr<Device, DeviceDeleter>;
using DeviceSharedPtr = std::shared_ptr<Device>;

bool checkHasChannel(const Device &, const ChannelInfo &);
bool checkHasCommand(const Device &, Command);
bool checkHasParameter(const Device &, Parameter);
std::size_t countChannelsWithType(const Device &, ChannelInfo::Type);
ParamAccess getParameterAccess(const Device &, Parameter);

}

#endif // DEVICE_H
