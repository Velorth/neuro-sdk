#include "device/callibri/callibri_mems_buffer.h"
#include "device/callibri/callibri_common_parameters.h"

namespace Neuro {

CallibriMemsBuffer::CallibriMemsBuffer(std::shared_ptr<CallibriCommonParameters> common_params):
        mCommonParameters(common_params) {
}

void CallibriMemsBuffer::onDataReceived(packet_number_t, const ByteBuffer &data) {
    ByteInterpreter<short> accelX;
    accelX.bytes[0] = data[DataShift];
    accelX.bytes[1] = data[DataShift + 1];
    auto accelXValue = accelX.value * doubleValue(mCommonParameters->accelerometerSens()) / 32767;

    ByteInterpreter<short> accelY;
    accelY.bytes[0] = data[DataShift + 2];
    accelY.bytes[1] = data[DataShift + 3];
    auto accelYValue = accelY.value * doubleValue(mCommonParameters->accelerometerSens()) / 32767;

    ByteInterpreter<short> accelZ;
    accelZ.bytes[0] = data[DataShift + 4];
    accelZ.bytes[1] = data[DataShift + 5];
    auto accelZValue = accelZ.value * doubleValue(mCommonParameters->accelerometerSens()) / 32767;

    ByteInterpreter<short> gyroX;
    gyroX.bytes[0] = data[DataShift + 6];
    gyroX.bytes[1] = data[DataShift + 7];
    auto gyroXValue = gyroX.value * doubleValue(mCommonParameters->gyroscopeSens()) / 32767;

    ByteInterpreter<short> gyroY;
    gyroY.bytes[0] = data[DataShift + 8];
    gyroY.bytes[1] = data[DataShift + 9];
    auto gyroYValue = gyroY.value * doubleValue(mCommonParameters->gyroscopeSens()) / 32767;

    ByteInterpreter<short> gyroZ;
    gyroZ.bytes[0] = data[DataShift + 10];
    gyroZ.bytes[1] = data[DataShift + 11];
    auto gyroZValue = gyroZ.value * doubleValue(mCommonParameters->gyroscopeSens()) / 32767;

    mMemsBuffer.append({MEMS{
                            {accelXValue, accelYValue, accelZValue},
                            {gyroXValue, gyroYValue, gyroZValue}}
                       });
}

const BaseBuffer<MEMS> &CallibriMemsBuffer::buffer() const {
    return mMemsBuffer;
}

}
