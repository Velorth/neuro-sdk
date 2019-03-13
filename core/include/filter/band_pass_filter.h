#ifndef BAND_PASS_FILTER_H
#define BAND_PASS_FILTER_H

#include <array>
#include "make_array.h"

namespace DSP {

template<std::size_t FreqLow, std::size_t FreqHigh, std::size_t FilterOrder, std::size_t SamplingFreq>
class BandPass;

}

#endif // BAND_PASS_FILTER_H
