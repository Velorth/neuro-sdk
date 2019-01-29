#ifndef CALLIBRI_IMPL_H
#define CALLIBRI_IMPL_H

#include "loop.h"
#include "callibri_protocol.h"
#include "callibri_command.h"
#include "device/device_impl.h"
#include "device/handler_chain.h"
#include "device/handler.h"
#include "device/packet.h"
#include "device/packet_sequence.h"
#include "device/request_scheduler.h"

namespace Neuro {

using CallibriRequestScheduler = RequestScheduler<CallibriCommandData>;
class CallibriCommonParameters;

class CallibriImpl : public DeviceImpl {
public:
    using param_changed_callback_t = std::function<void(Parameter)>;

    CallibriImpl(std::shared_ptr<BleDevice>,
                 std::shared_ptr<CallibriRequestScheduler>,
                 std::shared_ptr<CallibriCommonParameters>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<ParamPair> parameters() const override;
    ListenerPtr<void, Parameter> setParamChangedCallback(param_changed_callback_t) override;
    bool execute(Command) override;

	ListenerPtr<void, const int &>
		subscribeBatteryDataReceived(std::function<void(const int &)>, ChannelInfo) override;

	ListenerPtr<void, const std::vector<signal_sample_t> &>
		subscribeSignalDataReceived(std::function<void(const std::vector<signal_sample_t> &)>, ChannelInfo) override;

	ListenerPtr<void, const std::vector<resistance_sample_t> &>
		subscribeResistanceDataReceived(std::function<void(const std::vector<resistance_sample_t> &)>, ChannelInfo) override;

	ListenerPtr<void, const std::vector<MEMS> &>
		subscribeMEMSDataReceived(std::function<void(const std::vector<MEMS> &)>, ChannelInfo) override;

	ListenerPtr<void, const std::vector<Quaternion> &>
		subscribeOrientationDataReceived(std::function<void(const std::vector<Quaternion> &)>, ChannelInfo) override;

	ListenerPtr<void, const std::vector<double> &>
		subscribeRespirationDataReceived(std::function<void(const std::vector<double> &)>, ChannelInfo) override;

	ListenerPtr<void, const int &>
		subscribeConnectionStatsDataReceived(std::function<void(const int &)>, ChannelInfo) override;

	ListenerPtr<void, const std::vector<int> &>
		subscribePedometerDataReceived(std::function<void(const std::vector<int> &)>, ChannelInfo) override;

	ListenerPtr<void, const ElectrodeState &>
		subscribeElectrodesDataReceived(std::function<void(const ElectrodeState &)>, ChannelInfo) override;

private:
    static constexpr const char *class_name = "CallibriImpl";

	Notifier<void, const int &> mBatteryNotifier{ class_name };
	Notifier<void, const std::vector<signal_sample_t> &> mSignalNotifier{ class_name };
	Notifier<void, const std::vector<MEMS> &> mMEMSNotifier{ class_name };
	Notifier<void, const std::vector<Quaternion> &> mOrientationNotifier{ class_name };
	Notifier<void, const std::vector<double> &> mRespirationNotifier{ class_name };
	Notifier<void, const int &> mConnectionStatsNotifier{ class_name };
	Notifier<void, const std::vector<int> &> mPedometerNotifier{ class_name };
	Notifier<void, const ElectrodeState &> mElectrodesNotifier{ class_name };
    std::shared_ptr<CallibriRequestScheduler> mRequestHandler;
    std::shared_ptr<CallibriCommonParameters> mCommonParams;
	Notifier<void, Parameter> mParameterChangedNotifier;
    Loop<void(CallibriImpl*)> mBatteryNotificationLoop;
    Loop<void(CallibriImpl*)> mElectrodesNotificationLoop;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onCommandResponse(const ByteBuffer &);
    void onSignalReceived(const ByteBuffer &);
    void onMemsReceived(const ByteBuffer &);
    void onRespReceived(const ByteBuffer &);
    void onOrientationReceived(const ByteBuffer &);
    packet_number_t extractPacketNumber(const ByteBuffer &, std::size_t);
    void onParameterChanged(Parameter);
    void sendCommandPacket(std::shared_ptr<CallibriCommandData>);
    int convertVoltageToPercents(int);
    int requestBattryVoltage();
    bool isElectrodesAttached();
    int batteryChargePercents();
    void updateBatteryValue();
    void updateElectrodeState();

    template <Command Cmd>
    bool sendSimpleCommand(){
        auto cmdData = std::make_shared<CallibriCommandData>(toCallibriCommand<Cmd>());
        mRequestHandler->sendRequest(cmdData);
        cmdData->wait();

        return cmdData->getError() == CallibriError::NO_ERROR;
    }
};

}

#endif // CALLIBRI_IMPL_H
