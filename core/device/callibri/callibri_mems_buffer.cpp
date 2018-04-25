#include "device/callibri/callibri_mems_buffer.h"
#include "device/callibri/callibri_common_parameters.h"

namespace Neuro {

CallibriMemsBuffer::CallibriMemsBuffer(std::shared_ptr<CallibriCommonParameters> common_params):
        mCommonParameters(common_params) {
}

void CallibriMemsBuffer::onDataReceived(packet_number_t number, const ByteBuffer &data) {
    if (data.size() < MemsDataLength)
        return;

    ByteInterpreter<short> accelX;
    accelX.bytes[0] = data[0];
    accelX.bytes[1] = data[1];
    auto accelXValue = accelX.value * doubleValue(mCommonParameters->accelerometerSens()) / 32767;

    ByteInterpreter<short> accelY;
    accelY.bytes[0] = data[2];
    accelY.bytes[1] = data[3];
    auto accelYValue = accelY.value * doubleValue(mCommonParameters->accelerometerSens()) / 32767;

    ByteInterpreter<short> accelZ;
    accelZ.bytes[0] = data[4];
    accelZ.bytes[1] = data[5];
    auto accelZValue = accelZ.value * doubleValue(mCommonParameters->accelerometerSens()) / 32767;

    ByteInterpreter<short> gyroX;
    gyroX.bytes[0] = data[6];
    gyroX.bytes[1] = data[7];
    auto gyroXValue = gyroX.value * doubleValue(mCommonParameters->gyroscopeSens()) / 32767;

    ByteInterpreter<short> gyroY;
    gyroY.bytes[0] = data[8];
    gyroY.bytes[1] = data[9];
    auto gyroYValue = gyroY.value * doubleValue(mCommonParameters->gyroscopeSens()) / 32767;

    ByteInterpreter<short> gyroZ;
    gyroZ.bytes[0] = data[10];
    gyroZ.bytes[1] = data[11];
    auto gyroZValue = gyroZ.value * doubleValue(mCommonParameters->gyroscopeSens()) / 32767;

    /*auto packetsLost = mPacketSequence.onNewPacket(number);
    if (packetsLost > 0){
        std::vector<MEMS> zeroSamples(packetsLost, MEMS{});
        mMemsBuffer.append(zeroSamples);
    }*/

    mMemsBuffer.append({MEMS{
                            {accelXValue, accelYValue, accelZValue},
                            {gyroXValue, gyroYValue, gyroZValue}}
                       });
}

const BaseBuffer<MEMS> &CallibriMemsBuffer::buffer() const {
    return mMemsBuffer;
}

}
