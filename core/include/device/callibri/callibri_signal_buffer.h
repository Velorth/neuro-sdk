#ifndef CALLIBRI_SIGNAL_BUFFER_H
#define CALLIBRI_SIGNAL_BUFFER_H

#include "signal/safe_buffer.h"
#include "callibri_command.h"
#include "common_types.h"

namespace Neuro {

template<typename> class RequestHandler;
using CallibriRequestHandler = RequestHandler<CallibriCommandData>;

class CallibriSignalBuffer {
public:
    CallibriSignalBuffer(std::shared_ptr<CallibriRequestHandler>);
    CallibriSignalBuffer(const CallibriSignalBuffer &) = delete;
    CallibriSignalBuffer& operator=(const CallibriSignalBuffer &) = delete;

    void onSignalReceived(packet_number_t, const ByteBuffer &);
    const BaseBuffer<signal_sample_t>& buffer() const;

private:
    SafeBuffer<signal_sample_t, SignalBufferSize> mSignalBuffer;
};

}

#endif // CALLIBRI_SIGNAL_BUFFER_H
