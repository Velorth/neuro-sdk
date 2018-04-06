#ifndef BASE_BUFFER_H
#define BASE_BUFFER_H

#include <vector>

namespace Neuro {

template <typename SampleType>
class BaseBuffer {
public:
    virtual ~BaseBuffer() = default;

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
