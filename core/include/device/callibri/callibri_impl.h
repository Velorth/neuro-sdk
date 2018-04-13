#ifndef CALLIBRI_IMPL_H
#define CALLIBRI_IMPL_H

#include "device_impl.h"
#include "callibri_command.h"
#include "signal/safe_buffer.h"

namespace Neuro {

template<typename> class RequestHandler;
using CallibriRequestHandler = RequestHandler<CallibriCommandData>;

class CallibriCommonParameters;

class CallibriImpl : public DeviceImpl {
public:
    CallibriImpl(std::shared_ptr<BleDevice>,
                 std::shared_ptr<CallibriRequestHandler>,
                 std::shared_ptr<CallibriCommonParameters>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<std::pair<Parameter, ParamAccess> > parameters() const override;
    bool execute(Command) override;
    int batteryChargePercents() override;
    bool isElectrodesAttached() override;
    const BaseBuffer<signal_sample_t> &signalBuffer() const override;

private:
    static constexpr std::size_t SignalBufferSize = 360000; //10 minutes for 1kHz fsam
    std::shared_ptr<CallibriRequestHandler> mRequestHandler;
    std::shared_ptr<CallibriCommonParameters> mCommonParams;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onCommandPacketReceived(const unsigned char *const, size_t);
    void sendCommandPacket(std::shared_ptr<CallibriCommandData>);
    int requestBattryVoltage();
    int convertVoltageToPercents(int);
};

}

#endif // CALLIBRI_IMPL_H
