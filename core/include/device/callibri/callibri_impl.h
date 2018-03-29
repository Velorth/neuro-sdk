#ifndef CALLIBRI_IMPL_H
#define CALLIBRI_IMPL_H

#include "callibri_protocol.h"
#include "callibri_command.h"
#include "callibri_signal_buffer.h"
#include "callibri_respiration_buffer.h"
#include "callibri_mems_buffer.h"
#include "device/device_impl.h"
#include "device/handler_chain.h"
#include "device/handler.h"
#include "device/packet.h"

namespace Neuro {

template<typename> class RequestScheduler;
using CallibriRequestScheduler = RequestScheduler<CallibriCommandData>;
class CallibriPacketHandler;
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
    void setParamChangedCallback(param_changed_callback_t) override;
    bool execute(Command) override;
    int batteryChargePercents() override;
    bool isElectrodesAttached() override;
    const BaseBuffer<signal_sample_t> &signalBuffer() const override;    
    const BaseBuffer<resp_sample_t> &respirationBuffer() const override;
    const BaseBuffer<MEMS> &memsBuffer() const override;

private:
    static constexpr const char *class_name = "CallibriImpl";

    std::shared_ptr<CallibriRequestScheduler> mRequestHandler;
    std::shared_ptr<CallibriCommonParameters> mCommonParams;
    std::unique_ptr<CallibriSignalBuffer> mSignalBuffer;
    std::unique_ptr<CallibriRespirationBuffer> mRespirationBuffer;
    std::unique_ptr<CallibriMemsBuffer> mMemsBuffer;
    param_changed_callback_t parameterChangedCallback;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onCommandResponse(const ByteBuffer &);
    void onSignalReceived(const ByteBuffer &);
    void onMemsReceived(const ByteBuffer &);
    void onRespReceived(const ByteBuffer &);
    packet_number_t extractPacketNumber(const ByteBuffer &, std::size_t);
    void onParameterChanged(Parameter);
    void sendCommandPacket(std::shared_ptr<CallibriCommandData>);
    int convertVoltageToPercents(int);
    int requestBattryVoltage();
};

}

#endif // CALLIBRI_IMPL_H
