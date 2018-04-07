#ifndef CALLIBRI_BUFFER_COLLECTION_H
#define CALLIBRI_BUFFER_COLLECTION_H

#include <memory>
#include "callibri_signal_buffer.h"
#include "callibri_respiration_buffer.h"
#include "callibri_mems_buffer.h"
#include "callibri_orientation_buffer.h"

namespace Neuro {

class CallibriBufferCollection{
public:
    CallibriSignalBuffer &signalBuffer();
    CallibriRespirationBuffer &respirationBuffer();
    CallibriMemsBuffer &memsBuffer();
    CallibriOrientationBuffer &orientationBuffer();

    void setSignalBuffer(std::unique_ptr<CallibriSignalBuffer>&&);
    void setRespirationBuffer(std::unique_ptr<CallibriRespirationBuffer>&&);
    void setMemsBuffer(std::unique_ptr<CallibriMemsBuffer>&&);
    void setOrientationBuffer(std::unique_ptr<CallibriOrientationBuffer>&&);

    std::size_t packetsLost();
    std::size_t packetsReceived();

private:
    std::unique_ptr<CallibriSignalBuffer> mSignalBuffer;
    std::unique_ptr<CallibriRespirationBuffer> mRespirationBuffer;
    std::unique_ptr<CallibriMemsBuffer> mMemsBuffer;
    std::unique_ptr<CallibriOrientationBuffer> mOrientationBuffer;
};

}

#endif // CALLIBRI_BUFFER_COLLECTION_H
