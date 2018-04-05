#ifndef CALLIBRI_RESPIRATION_BUFFER_H
#define CALLIBRI_RESPIRATION_BUFFER_H

#include "signal/safe_buffer.h"
#include "common_types.h"
#include "callibri_buffer.h"

namespace Neuro {

class CallibriRespirationBuffer : public CallibriBuffer<65500> {
public:
    CallibriRespirationBuffer() = default;
    CallibriRespirationBuffer(const CallibriRespirationBuffer &) = delete;
    CallibriRespirationBuffer& operator=(const CallibriRespirationBuffer &) = delete;

    void onDataReceived(packet_number_t, const ByteBuffer &);
    const BaseBuffer<resp_sample_t>& buffer() const;

private:
    static constexpr std::size_t RespirationDataLength = 3;
    static constexpr std::size_t RespirationBufferSize = 12000; //10 minutes for 20 Hz fsam
    static constexpr resp_sample_t MSBValue = 0.288e-6;

    SafeBuffer<resp_sample_t, RespirationBufferSize> mRespirationBuffer;
};

}
#endif // CALLIBRI_RESPIRATION_BUFFER_H
