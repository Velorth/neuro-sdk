#ifndef BRAINBIT_IMPL_H
#define BRAINBIT_IMPL_H

#include "device/device_impl.h"
#include "device/request_scheduler.h"
#include "brainbit_protocol.h"
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
    const BaseBuffer<Quaternion> &angleBuffer() const override;

private:
    using BrainbitRequestHandler = RequestScheduler<BrainbitCommandData>;

    static constexpr const char *class_name = "BrainbitImpl";
    static constexpr std::size_t SignalBufferSize = 360000; //10 minutes for 250 Hz fsam and 4 channels

    std::unique_ptr<BrainbitRequestHandler> mRequestHandler;
    BrainbitCommand mBrainbitState;
    int mBatteryPercents;
    SafeBuffer<signal_sample_t, SignalBufferSize> mSignalBuffer;
    PacketSequence<2047> mPacketCounter;
    param_changed_callback_t parameterChangedCallback;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onParameterChanged(Parameter);
    void parseBattery(const ByteBuffer &);
    void parseState(BrainbitCommand cmd, const ByteBuffer &);
    bool execStartSignalCommand();
    bool execStopSignalCommand();
    bool execStartResistCommand();
    bool execStopResistCommand();
    bool stopAll();
    void sendCommandPacket(std::shared_ptr<BrainbitCommandData>);
};

}

#endif // BRAINBIT_IMPL_H
