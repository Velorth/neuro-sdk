#ifndef BRAINBIT_RESISTANCE_BUFFER_H
#define BRAINBIT_RESISTANCE_BUFFER_H

#include <mutex>
#include <vector>
#include "signal/base_buffer.h"
#include "signal/safe_buffer.h"
#include "common_types.h"

namespace Neuro {

class BrainbitResistanceBuffer final : public BaseBuffer<resistance_sample_t>{
public:
    BrainbitResistanceBuffer();

    length_listener_ptr subscribeLengthChanged(length_callback_t callback) const noexcept override;
    void append(const std::vector<resistance_sample_t> &) override;
    std::vector<resistance_sample_t> readAvailable(std::size_t global_offset, std::size_t length) const override;
    std::vector<resistance_sample_t> readFill(std::size_t global_offset, std::size_t length, resistance_sample_t fill_value) const override;
    std::vector<resistance_sample_t> rawBuffer() const override;
    std::size_t bufferSize() const noexcept override;
    std::size_t totalLength() const noexcept override;
    void reset() override;

private:
    static constexpr std::size_t ResistanceBufferSize = 360000;
    static constexpr std::size_t ChannelsCount = 4;
    static constexpr std::size_t AveragingSamplesCount = 100 * ChannelsCount;

    SafeBuffer<resistance_sample_t, ResistanceBufferSize> mMainBuffer;
    std::mutex mInputBufferMutex;
    std::vector<resistance_sample_t> mInputBuffer;

    void flushBufferData();
};

}

#endif // BRAINBIT_RESISTANCE_BUFFER_H
