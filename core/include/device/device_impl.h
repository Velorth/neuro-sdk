#ifndef DEVICE_IMPL_H
#define DEVICE_IMPL_H

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include "common_types.h"
#include "task_queue.h"
#include "event_notifier.h"

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
enum class ElectrodeState;

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

    ListenerPtr<void, const std::vector<int> &>
    subscribeBatteryDataReceived(std::function<void(const std::vector<int> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<signal_sample_t> &>
    subscribeSignalDataReceived(std::function<void(const std::vector<signal_sample_t> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<resistance_sample_t> &>
    subscribeResistanceDataReceived(std::function<void(const std::vector<resistance_sample_t> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<MEMS> &>
    subscribeMEMSDataReceived(std::function<void(const std::vector<MEMS> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<Quaternion> &>
    subscribeOrientationDataReceived(std::function<void(const std::vector<Quaternion> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<double> &>
    subscribeRespirationDataReceived(std::function<void(const std::vector<double> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<int> &>
    subscribeConnectionStatsDataReceived(std::function<void(const std::vector<int> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<int> &>
    subscribePedometerDataReceived(std::function<void(const std::vector<int> &)>, ChannelInfo);

    ListenerPtr<void, const std::vector<ElectrodeState> &>
    subscribeElectrodesDataReceived(std::function<void(const std::vector<ElectrodeState> &)>, ChannelInfo);

protected:    
    std::shared_ptr<BleDevice> mBleDevice;

    DeviceImpl(std::shared_ptr<BleDevice>,
               std::unique_ptr<ParameterReader>,
               std::unique_ptr<ParameterWriter>);

    Notifier<void, const std::vector<int> &> mBatteryNotifier{class_name};
    std::unordered_map<std::size_t, Notifier<void, const std::vector<signal_sample_t> &>> mSignalNotifierMap;
    std::unordered_map<std::size_t, Notifier<void, const std::vector<resistance_sample_t> &>> mResistanceNotifierMap;
    Notifier<void, const std::vector<MEMS> &> mMEMSNotifier{class_name};
    Notifier<void, const std::vector<Quaternion> &> mOrientationNotifier{class_name};
    Notifier<void, const std::vector<double> &> mRespirationNotifier{class_name};
    Notifier<void, const std::vector<int> &> mConnectionStatsNotifier{class_name};
    Notifier<void, const std::vector<int> &> mPedometerNotifier{class_name};
    Notifier<void, const std::vector<ElectrodeState> &> mElectrodesNotifier{class_name};

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
