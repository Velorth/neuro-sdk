#ifndef BRAINBIT_IMPL_H
#define BRAINBIT_IMPL_H

#include "device/device_impl.h"
#include "device/request_scheduler.h"
#include "brainbit_protocol.h"
#include "brainbit_resistance_buffer.h"
#include "signal/safe_buffer.h"
#include "device/packet_sequence.h"

namespace Neuro {

class BrainbitImpl : public DeviceImpl {
public:
    using param_changed_callback_t = std::function<void(Parameter)>;
    BrainbitImpl(std::shared_ptr<BleDevice>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<std::pair<Parameter, ParamAccess>> parameters() const override;
    void setParamChangedCallback(param_changed_callback_t) override;
    bool execute(Command) override;
    int batteryChargePercents() override;
    bool isElectrodesAttached() override;    
    std::size_t packetsLost() override;
    std::size_t packetsReceived() override;
    const BaseBuffer<signal_sample_t> &signalBuffer() const override;
    const BaseBuffer<resp_sample_t> &respirationBuffer() const override;
    const BaseBuffer<MEMS> &memsBuffer() const override;
    const BaseBuffer<Quaternion> &orientationBuffer() const override;

private:
    using BrainbitRequestHandler = RequestScheduler<BrainbitCommandData>;

    static constexpr const char *class_name = "BrainbitImpl";
    static constexpr std::size_t SignalBufferSize = 360000; //10 minutes for 250 Hz fsam and 4 channels

    std::unique_ptr<BrainbitRequestHandler> mRequestHandler;
    BrainbitCommand mBrainbitState;
    int mBatteryPercents{0};
    SafeBuffer<signal_sample_t, SignalBufferSize> mSignalBuffer;
    PacketSequence<2048> mPacketCounter;
    param_changed_callback_t parameterChangedCallback;
    std::vector<ChannelInfo> mChannels;
    std::size_t mCurrentResistChannel{0};
    std::vector<resistance_sample_t> mResistBuffer;

    void initChannels();
    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onParameterChanged(Parameter);
    void parseBattery(const ByteBuffer &);
    void parseState(BrainbitCommand cmd, const ByteBuffer &);
    void parseSignalData(const ByteBuffer &);
    void onSignalReceived(const std::vector<signal_sample_t> &);
    void onResistanceReceived(const std::vector<resistance_sample_t> &);
    bool execStartSignalCommand();
    bool execStopSignalCommand();
    bool startResist();
    bool execStartResistCommand(std::size_t);
    bool execStopResistCommand();
    bool stopAll();
    void sendCommandPacket(std::shared_ptr<BrainbitCommandData>);
};

}

#endif // BRAINBIT_IMPL_H
