#ifndef BASE_BUFFER_H
#define BASE_BUFFER_H

#include <functional>
#include <vector>
#include "event_listener.h"
#include "common_types.h"

namespace Neuro {

template <typename SampleType>
class BaseBuffer {
public:
    using length_callback_t = std::function<void(data_length_t)>;
    using length_listener_ptr = ListenerPtr<void, data_length_t>;

    virtual ~BaseBuffer() = default;

    virtual length_listener_ptr subscribeLengthChanged(length_callback_t callback) const noexcept = 0;
    virtual void append(const std::vector<SampleType> &) = 0;
    virtual std::vector<SampleType> readAvailable(std::size_t global_offset, std::size_t length) const = 0;
    virtual std::vector<SampleType> readFill(std::size_t global_offset, std::size_t length, SampleType fill_value) const = 0;
    virtual std::vector<SampleType> rawBuffer() const = 0;
    virtual std::size_t bufferSize() const noexcept = 0;
    virtual std::size_t totalLength() const noexcept = 0;
    virtual void reset() = 0;
};

}

#endif // BASE_BUFFER_H
