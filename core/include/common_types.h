#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <vector>

namespace Neuro {

using data_offset_t = std::size_t;
using data_length_t = std::size_t;
using sampling_frequency_t = float;
using signal_sample_t = double;
using resp_sample_t = double;
using packet_number_t = unsigned int;
using Byte = unsigned char;
using ByteBuffer = std::vector<Byte>;

template <typename T>
union ByteInterpreter
{
    T value;
    Byte bytes[sizeof(T)];
};

}

#endif // COMMON_TYPES_H
