#ifndef BRAINBIT_IMPL_H
#define BRAINBIT_IMPL_H

#include "device/device_impl.h"
#include "device/request_handler.h"
#include "brainbit_command.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class BrainbitImpl : public DeviceImpl {
public:
    BrainbitImpl(std::shared_ptr<BleDevice>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<std::pair<Parameter, ParamAccess> > parameters() const override;
    bool execute(Command) override;
    int batteryChargePercents() override;
    bool isElectrodesAttached() override;    
    const BaseBuffer<signal_sample_t> &signalBuffer() const override;

private:
    using BrainbitRequestHandler = RequestHandler<BrainbitCommandData>;
    static constexpr std::size_t SignalBufferSize = 360000; //10 minutes for 250 Hz fsam and 4 channels
    std::unique_ptr<BrainbitRequestHandler> mRequestHandler;
    BrainbitCommand mBrainbitState;
    int mBatteryPercents;    
    SafeBuffer<signal_sample_t, SignalBufferSize> mSignalBuffer;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
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
