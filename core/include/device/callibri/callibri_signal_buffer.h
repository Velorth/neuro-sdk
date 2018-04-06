#ifndef CALLIBRI_SIGNAL_BUFFER_H
#define CALLIBRI_SIGNAL_BUFFER_H

#include <memory>
#include "signal/safe_buffer.h"
#include "common_types.h"
#include "callibri_buffer.h"

namespace Neuro {

class CallibriCommonParameters;

class CallibriSignalBuffer : public CallibriBuffer<65500> {
public:
    CallibriSignalBuffer(std::shared_ptr<CallibriCommonParameters>);
    CallibriSignalBuffer(const CallibriSignalBuffer &) = delete;
    CallibriSignalBuffer& operator=(const CallibriSignalBuffer &) = delete;

    void onDataReceived(packet_number_t, const ByteBuffer &);
    const BaseBuffer<signal_sample_t>& buffer() const;

private:    
    static constexpr std::size_t SignalDataLength = 18;
    static constexpr std::size_t SignalBufferSize = 150000; //10 minutes for 250 Hz fsam
    static constexpr int AdcCapacity = 8388607;
    static constexpr double ReferenceVoltage = 2.42;

    std::shared_ptr<CallibriCommonParameters> mCommonParameters;
    SafeBuffer<signal_sample_t, SignalBufferSize> mSignalBuffer;
};

}

#endif // CALLIBRI_SIGNAL_BUFFER_H
