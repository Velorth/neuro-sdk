#include "device/callibri/callibri_orientation_buffer.h"
#include "device/callibri/callibri_common_parameters.h"

namespace Neuro {

void CallibriOrientationBuffer::onDataReceived(packet_number_t number, const ByteBuffer &data) {
    if (data.size() < AngleDataLength)
        return;

    Quaternion quat;
    {
        ByteInterpreter<float> w;
        w.bytes[0] = data[0];
        w.bytes[1] = data[1];
        w.bytes[2] = data[2];
        w.bytes[3] = data[3];
        quat.W = w.value;
    }

    {
        ByteInterpreter<float> x;
        x.bytes[0] = data[4];
        x.bytes[1] = data[5];
        x.bytes[2] = data[6];
        x.bytes[3] = data[7];
        quat.X = x.value;
    }

    {
        ByteInterpreter<float> y;
        y.bytes[0] = data[8];
        y.bytes[1] = data[9];
        y.bytes[2] = data[10];
        y.bytes[3] = data[11];
        quat.Y = y.value;
    }

    {
        ByteInterpreter<float> z;
        z.bytes[0] = data[12];
        z.bytes[1] = data[13];
        z.bytes[2] = data[14];
        z.bytes[3] = data[15];
        quat.Z = z.value;
    }

    /*auto packetsLost = mPacketSequence.onNewPacket(number);
    if (packetsLost > 0){
        std::vector<Quaternion> zeroSamples(packetsLost, Quaternion{});
        mOrientationBuffer.append(zeroSamples);
    }*/

    mOrientationBuffer.append({quat});
}

const BaseBuffer<Quaternion> &CallibriOrientationBuffer ::buffer() const {
    return mOrientationBuffer;
}

}
