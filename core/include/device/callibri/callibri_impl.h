#ifndef CALLIBRI_IMPL_H
#define CALLIBRI_IMPL_H

#include "callibri_protocol.h"
#include "callibri_command.h"
#include "device/device_impl.h"
#include "device/handler_chain.h"
#include "device/handler.h"
#include "device/packet.h"

namespace Neuro {

template<typename> class RequestHandler;
using CallibriRequestHandler = RequestHandler<CallibriCommandData>;

class CallibriCommonParameters;

class CallibriImpl : public DeviceImpl {
public:
    using param_changed_callback_t = std::function<void(Parameter)>;

    CallibriImpl(std::shared_ptr<BleDevice>,
                 std::shared_ptr<CallibriRequestHandler>,
                 std::shared_ptr<CallibriCommonParameters>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<ParamPair> parameters() const override;
    void setParamChangedCallback(param_changed_callback_t) override;
    bool execute(Command) override;
    int batteryChargePercents() override;
    bool isElectrodesAttached() override;
    const BaseBuffer<signal_sample_t> &signalBuffer() const override;    
    const BaseBuffer<resp_sample_t> &respirationBuffer() const override;
    const BaseBuffer<MEMS> &memsBuffer() const override;

private:
    using CallibriPacketHandler = Handler<Packet<CallibriPacketType>>;

    static constexpr const char *class_name = "CallibriImpl";

    std::shared_ptr<CallibriRequestHandler> mRequestHandler;
    std::shared_ptr<HandlerChain<CallibriPacketHandler>> mPacketHandler;
    std::shared_ptr<CallibriCommonParameters> mCommonParams;
    param_changed_callback_t parameterChangedCallback;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onParameterChanged(Parameter);
    void onCommandPacketReceived(const unsigned char *const, std::size_t);
    void sendCommandPacket(std::shared_ptr<CallibriCommandData>);
    int requestBattryVoltage();
    int convertVoltageToPercents(int);
};

}

#endif // CALLIBRI_IMPL_H
