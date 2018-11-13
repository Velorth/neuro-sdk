#ifndef CALLIBRI_ANGLE_BUFFER_H
#define CALLIBRI_ANGLE_BUFFER_H

#include "signal/safe_buffer.h"
#include "channels/info/quaternion.h"
#include "common_types.h"

namespace Neuro {

class CallibriOrientationBuffer {
public:
    CallibriOrientationBuffer() = default;
    CallibriOrientationBuffer(const CallibriOrientationBuffer &) = delete;
    CallibriOrientationBuffer& operator=(const CallibriOrientationBuffer &) = delete;

    void onDataReceived(packet_number_t, const ByteBuffer &);
    const BaseBuffer<Quaternion>& buffer() const;

private:
    static constexpr std::size_t AngleDataLength = 16;
    static constexpr std::size_t OrientationBufferSize = 60000; //10 minutes for 100 Hz fsam

    SafeBuffer<Quaternion, OrientationBufferSize> mOrientationBuffer;
};

}

#endif // CALLIBRI_ANGLE_BUFFER_H
