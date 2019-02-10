#ifndef BRAINBIT_IMPL_H
#define BRAINBIT_IMPL_H

#include "device/device_impl.h"
#include "device/request_scheduler.h"
#include "brainbit_protocol.h"
#include "signal/safe_buffer.h"
#include "device/packet_sequence.h"
#include "device/device_parameters.h"
#include "loop.h"

namespace Neuro {

class BrainbitImpl : public DeviceImpl {
public:
	struct BrainbitParameterSetter {
		FirmwareVersion mFirmwareVersion{0, 0};
	};

    using param_changed_callback_t = std::function<void(Parameter)>;
    BrainbitImpl(std::shared_ptr<BleDevice>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<std::pair<Parameter, ParamAccess>> parameters() const override;
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
    using BrainbitRequestHandler = RequestScheduler<BrainbitCommandData>;

    static constexpr const char *class_name = "BrainbitImpl";

	Notifier<void, const int &> mBatteryNotifier{ class_name };
	std::unordered_map<std::size_t, Notifier<void, const std::vector<signal_sample_t> &>> mSignalNotifierMap;
	std::unordered_map<std::size_t, Notifier<void, const std::vector<resistance_sample_t> &>> mResistanceNotifierMap;
	Notifier<void, const int &> mConnectionStatsNotifier{ class_name };

    std::unique_ptr<BrainbitRequestHandler> mRequestHandler;
    BrainbitCommand mBrainbitState;
    int mBatteryPercents{0};
    Notifier<void, Parameter> mParameterChangedNotifier;
    std::vector<ChannelInfo> mChannels;
    std::size_t mCurrentResistChannel{0};
    std::vector<resistance_sample_t> mResistBuffer;
	BrainbitParameterSetter mSetter;
	Loop<void(BrainbitImpl*)> mBatteryNotificationLoop;

    void initChannels();
    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onParameterChanged(Parameter);
    void parseBattery(const ByteBuffer &);
    void parseState(BrainbitCommand cmd, const ByteBuffer &);
	void parseVersion(const ByteBuffer &);
    void parseSignalData(const ByteBuffer &);
    void onSignalReceived(const std::vector<signal_sample_t> &);
    void onResistanceReceived(const std::vector<resistance_sample_t> &);
    void onResistanceCalculated(resistance_sample_t value);
    bool execStartSignalCommand();
    bool execStopSignalCommand();
    bool startResist();
    bool execStartResistCommand(std::size_t);
    bool execStopResistCommand();
    bool stopAll();
    void sendCommandPacket(std::shared_ptr<BrainbitCommandData>);
    void notifyBatteryState();
};

}

#endif // BRAINBIT_IMPL_H
