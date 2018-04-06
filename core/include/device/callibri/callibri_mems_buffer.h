#ifndef CALLIBRI_MEMS_BUFFER_H
#define CALLIBRI_MEMS_BUFFER_H

#include <memory>
#include "channels/mems_data.h"
#include "common_types.h"
#include "callibri_buffer.h"
#include "signal/safe_buffer.h"

namespace Neuro {

class CallibriCommonParameters;

class CallibriMemsBuffer : public CallibriBuffer<65500> {
public:
    CallibriMemsBuffer(std::shared_ptr<CallibriCommonParameters>);
    CallibriMemsBuffer(const CallibriMemsBuffer &) = delete;
    CallibriMemsBuffer& operator=(const CallibriMemsBuffer &) = delete;

    void onDataReceived(packet_number_t, const ByteBuffer &);
    const BaseBuffer<MEMS>& buffer() const;

private:
    static constexpr std::size_t MemsDataLength = 16;
    static constexpr std::size_t MemsBufferSize = 60000; //10 minutes for 100 Hz fsam

    std::shared_ptr<CallibriCommonParameters> mCommonParameters;
    SafeBuffer<MEMS, MemsBufferSize> mMemsBuffer;
};

}

#endif // CALLIBRI_MEMS_BUFFER_H
