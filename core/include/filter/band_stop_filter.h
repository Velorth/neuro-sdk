#ifndef BAND_STOP_FILTER_H
#define BAND_STOP_FILTER_H

#include <array>
#include "make_array.h"

namespace DSP {

template<std::size_t FreqLow, std::size_t FreqHigh, std::size_t FilterOrder, std::size_t SamplingFreq>
class BandStop;

template<>
class BandStop<45, 55, 4, 250>{
protected:
    static constexpr std::size_t Order = 4;
    std::array<double, Order> Denominators{{
            0.7008967811884026,
            -0.94976030879978701,
            1.9723023606063141,
            -1.136085493907057}};
    std::array<double, Order + 1> Numerators{{
            0.8370891905663449,
            -1.0429229013534211,
            1.9990207606620285,
            -1.0429229013534211,
            0.8370891905663449}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

}

#endif // BAND_STOP_FILTER_H
