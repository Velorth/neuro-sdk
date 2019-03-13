#ifndef HIGH_PASS_FILTER_H
#define HIGH_PASS_FILTER_H

#include <array>
#include "make_array.h"

namespace DSP {

template<std::size_t CutoffFreq, std::size_t FilterOrder, std::size_t SamplingFreq>
class HighPass;

template<>
class HighPass<5, 3, 125>{
protected:
    static constexpr std::size_t Order = 3;
    std::array<double, Order> Denominators{{
            -.6041096995072748,
            2.115254127003159,
            -2.498608344691178}};
    std::array<double, Order + 1> Numerators{{
            -.7772465214002014,
            2.331739564200604,
            -2.331739564200604,
            .7772465214002014}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class HighPass<3, 2, 125>{
protected:
    static constexpr std::size_t Order = 2;
    std::array<double, Order> Denominators{{
            .8674721337916686,
            -1.858043298700259}};
    std::array<double, Order + 1> Numerators{{
            .931378858122982,
            -1.862757716245964,
            .931378858122982}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class HighPass<11, 3, 125>{
protected:
    static constexpr std::size_t Order = 3;
    std::array<double, Order> Denominators{{
             -.3258789841198803,
             1.336195449676777,
             -1.905982115789151}};
    std::array<double, Order + 1> Numerators{{
            -.571007068698226,
            1.713021206094678,
            -1.713021206094678,
            .571007068698226}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class HighPass<2, 2, 250>{
protected:
    static constexpr std::size_t Order = 2;
    std::array<double, Order> Denominators{{
            0.93138168212690264,
            -1.9289422632520334}};
    std::array<double, Order + 1> Numerators{{
            0.96508098634473405,
            -1.9301619726894681,
            0.96508098634473405}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

}

#endif // HIGH_PASS_FILTER_H
