#ifndef DEVICE_IMPL_H
#define DEVICE_IMPL_H

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include "common_types.h"
#include "task_queue.h"
#include "event_notifier.h"
#include "channels/info/channel_info.h"

namespace Neuro {

class BleDevice;
enum class Command;
enum class Parameter;
enum class ParamAccess;
class ParameterReader;
class ParameterWriter;
template <typename>
class BaseBuffer;

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

	virtual ChannelDataListenerType<ChannelInfo::Type::Battery>
    subscribeBatteryDataReceived(ChannelDataCallbackFunctionType<ChannelInfo::Type::Battery>, ChannelInfo) = 0;

	virtual ListenerPtr<void, const std::vector<signal_sample_t> &>
    subscribeSignalDataReceived(std::function<void(const std::vector<signal_sample_t> &)>, ChannelInfo) = 0;

	virtual ListenerPtr<void, const std::vector<resistance_sample_t> &>
    subscribeResistanceDataReceived(std::function<void(const std::vector<resistance_sample_t> &)>, ChannelInfo) = 0;

	virtual ListenerPtr<void, const std::vector<MEMS> &>
    subscribeMEMSDataReceived(std::function<void(const std::vector<MEMS> &)>, ChannelInfo) = 0;

	virtual ListenerPtr<void, const std::vector<Quaternion> &>
    subscribeOrientationDataReceived(std::function<void(const std::vector<Quaternion> &)>, ChannelInfo) = 0;

	virtual ListenerPtr<void, const std::vector<double> &>
    subscribeRespirationDataReceived(std::function<void(const std::vector<double> &)>, ChannelInfo) = 0;

	virtual ChannelDataListenerType<ChannelInfo::Type::ConnectionStats>
    subscribeConnectionStatsDataReceived(ChannelDataCallbackFunctionType<ChannelInfo::Type::ConnectionStats>, ChannelInfo) = 0;

	virtual ListenerPtr<void, const std::vector<int> &>
    subscribePedometerDataReceived(std::function<void(const std::vector<int> &)>, ChannelInfo) = 0;

	virtual ChannelDataListenerType<ChannelInfo::Type::ElectrodesState>
    subscribeElectrodesDataReceived(ChannelDataCallbackFunctionType<ChannelInfo::Type::ElectrodesState>, ChannelInfo) = 0;

protected:    
    std::shared_ptr<BleDevice> mBleDevice;

    DeviceImpl(std::shared_ptr<BleDevice>,
               std::unique_ptr<ParameterReader>,
               std::unique_ptr<ParameterWriter>);

private:
    friend class Device;
    static constexpr const char *class_name = "DeviceImpl";

    std::unique_ptr<ParameterReader> mParamReader;
    std::unique_ptr<ParameterWriter> mParamWriter;
    TaskQueue mDataReceivedQueue{"DataReceived"};
    TaskQueue mStatusReceivedQueue{"StatusReceived"};

    void subscribeDataReceived();
    void subscribeStatusReceived();
    virtual void onDataReceived(const ByteBuffer &) = 0;
    virtual void onStatusDataReceived(const ByteBuffer &) = 0;
};



}

#endif // DEVICE_IMPL_H
