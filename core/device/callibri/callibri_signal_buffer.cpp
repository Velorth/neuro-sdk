#include "device/callibri/callibri_protocol.h"
#include "device/callibri/callibri_signal_buffer.h"
#include "device/callibri/callibri_common_parameters.h"

namespace Neuro {

CallibriSignalBuffer::CallibriSignalBuffer(std::shared_ptr<CallibriCommonParameters> common_params):
        mCommonParameters(common_params) {
}

void CallibriSignalBuffer::onDataReceived(packet_number_t, const ByteBuffer &data) {
    std::vector<signal_sample_t> samples;
    for (auto sample = data.begin(); sample!=data.end(); sample += 2) {
        ByteInterpreter<short> shortSample;
        shortSample.bytes[0] = *sample;
        shortSample.bytes[1] = *(sample + 1);

        auto adcValue = shortSample.value;
        double sampleValue = (ReferenceVoltage / AdcCapacity) * adcValue /
                intValue(mCommonParameters->gain());

        samples.push_back(sampleValue);
    }

    mSignalBuffer.append(samples);
}

const BaseBuffer<signal_sample_t> &CallibriSignalBuffer::buffer() const {
    return mSignalBuffer;
}

}
