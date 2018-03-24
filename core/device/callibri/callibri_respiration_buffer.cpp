#include "device/callibri/callibri_protocol.h"
#include "device/callibri/callibri_respiration_buffer.h"
#include "device/callibri/callibri_common_parameters.h"

namespace Neuro {

void CallibriRespirationBuffer::onSignalReceived(packet_number_t, const ByteBuffer &data) {
    if (data.size() < RespirationDataLength)
        return;

    static_assert(sizeof(long) >= RespirationDataLength,
                  "Data length type does not fit into union value type");

    ByteInterpreter<long> respData;
    for (std::size_t i = 0; i < RespirationDataLength; ++i){
        respData.bytes[i] = data[i];
    }
    auto value = respData.value * MSBValue;
    mRespirationBuffer.append({value});
}

const BaseBuffer<resp_sample_t> &CallibriRespirationBuffer ::buffer() const {
    return mRespirationBuffer ;
}

}
