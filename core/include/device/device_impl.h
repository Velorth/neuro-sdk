#ifndef DEVICE_IMPL_H
#define DEVICE_IMPL_H

#include <memory>
#include <functional>
#include <vector>
#include "common_types.h"

namespace Neuro {

class BleDevice;
class ChannelInfo;
enum class Command;
enum class Parameter;
enum class ParamAccess;
class ParameterReader;
class ParameterWriter;
template <typename>
class BaseBuffer;
struct MEMS;
struct Quaternion;

class DeviceImpl {
public:
    virtual ~DeviceImpl();

    void connect();
    void disconnect();
    virtual std::vector<ChannelInfo> channels() const = 0;
    virtual std::vector<Command> commands() const = 0;
    virtual std::vector<std::pair<Parameter, ParamAccess>> parameters() const = 0;
    virtual void setParamChangedCallback(std::function<void(Parameter)>) = 0;
    virtual bool execute(Command) = 0;
    virtual int batteryChargePercents() = 0;
    virtual bool isElectrodesAttached() = 0;
    virtual std::size_t packetsLost() = 0;
    virtual std::size_t packetsReceived() = 0;
    virtual const BaseBuffer<signal_sample_t> &signalBuffer() const = 0;
    virtual const BaseBuffer<resp_sample_t> &respirationBuffer() const = 0;
    virtual const BaseBuffer<MEMS> &memsBuffer() const = 0;
    virtual const BaseBuffer<Quaternion> &orientationBuffer() const = 0;

protected:    
    std::shared_ptr<BleDevice> mBleDevice;

    DeviceImpl(std::shared_ptr<BleDevice>,
               std::unique_ptr<ParameterReader>,
               std::unique_ptr<ParameterWriter>);

private:
    friend class Device;

    std::unique_ptr<ParameterReader> mParamReader;
    std::unique_ptr<ParameterWriter> mParamWriter;

    void subscribeDataReceived();
    void subscribeStatusReceived();
    virtual void onDataReceived(const ByteBuffer &) = 0;
    virtual void onStatusDataReceived(const ByteBuffer &) = 0;
};



}

#endif // DEVICE_IMPL_H
