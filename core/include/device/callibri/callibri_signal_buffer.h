#ifndef CALLIBRI_SIGNAL_BUFFER_H
#define CALLIBRI_SIGNAL_BUFFER_H

#include <memory>
#include "signal/safe_buffer.h"
#include "common_types.h"

namespace Neuro {

class CallibriCommonParameters;

class CallibriSignalBuffer {
public:
    CallibriSignalBuffer(std::shared_ptr<CallibriCommonParameters>);
    CallibriSignalBuffer(const CallibriSignalBuffer &) = delete;
    CallibriSignalBuffer& operator=(const CallibriSignalBuffer &) = delete;

    void onSignalReceived(packet_number_t, const ByteBuffer &);
    const BaseBuffer<signal_sample_t>& buffer() const;

private:
    static constexpr std::size_t SignalBufferSize = 120000; //10 minutes for 250 Hz fsam

    std::shared_ptr<CallibriCommonParameters> mCommonParameters;
    SafeBuffer<signal_sample_t, SignalBufferSize> mSignalBuffer;
};

}

#endif // CALLIBRI_SIGNAL_BUFFER_H
