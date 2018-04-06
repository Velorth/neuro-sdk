#ifndef CALLIBRI_IMPL_H
#define CALLIBRI_IMPL_H

#include "callibri_protocol.h"
#include "callibri_command.h"
#include "device/device_impl.h"
#include "device/handler_chain.h"
#include "device/handler.h"
#include "device/packet.h"
#include "device/request_scheduler.h"

namespace Neuro {

using CallibriRequestScheduler = RequestScheduler<CallibriCommandData>;
class CallibriPacketHandler;
class CallibriCommonParameters;
class CallibriBufferCollection;

class CallibriImpl : public DeviceImpl {
public:
    using param_changed_callback_t = std::function<void(Parameter)>;

    CallibriImpl(std::shared_ptr<BleDevice>,
                 std::shared_ptr<CallibriRequestScheduler>,
                 std::shared_ptr<CallibriCommonParameters>,
                 std::shared_ptr<CallibriBufferCollection>);

    std::vector<ChannelInfo> channels() const override;
    std::vector<Command> commands() const override;
    std::vector<ParamPair> parameters() const override;
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
    static constexpr const char *class_name = "CallibriImpl";

    std::shared_ptr<CallibriRequestScheduler> mRequestHandler;
    std::shared_ptr<CallibriCommonParameters> mCommonParams;
    std::shared_ptr<CallibriBufferCollection> mBufferCollection;
    param_changed_callback_t parameterChangedCallback;

    void onDataReceived(const ByteBuffer &) override;
    void onStatusDataReceived(const ByteBuffer &) override;
    void onCommandResponse(const ByteBuffer &);
    void onSignalReceived(const ByteBuffer &);
    void onMemsReceived(const ByteBuffer &);
    void onRespReceived(const ByteBuffer &);
    void onAngleReceived(const ByteBuffer &);
    packet_number_t extractPacketNumber(const ByteBuffer &, std::size_t);
    void onParameterChanged(Parameter);
    void sendCommandPacket(std::shared_ptr<CallibriCommandData>);
    int convertVoltageToPercents(int);
    int requestBattryVoltage();

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
