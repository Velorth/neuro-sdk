#ifndef DEVICE_H
#define DEVICE_H

#include <memory>
#include <vector>
#include <functional>
#include "device_parameters.h"

namespace Neuro {

class ChannelInfo;
class DeviceImpl;

template <Parameter>
struct ParamValue;

class Device final {
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

private:
    friend class DeviceFactory;
    friend class BatteryChannel;
    friend class ElectrodeStateChannel;
    friend class SignalChannel;
    friend class RespirationChannel;
    friend class MemsChannel;
    friend class AngleChannel;
    Device(std::unique_ptr<DeviceImpl>);
};

bool checkHasChannel(const Device &, const ChannelInfo &);
bool checkHasCommand(const Device &, Command);
bool checkHasParameter(const Device &, Parameter);
ParamAccess getParameterAccess(const Device &, Parameter);

}

#endif // DEVICE_H
