#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#include <array>
#include "make_array.h"

namespace DSP {

template<std::size_t CutoffFreq, std::size_t FilterOrder, std::size_t SamplingFreq>
class LowPass;

template<>
class LowPass<27, 4, 125>{
protected:
    static constexpr std::size_t Order = 4;
    std::array<double, Order> Denominators{{
        6.615274550600441e-03,
        4.458424700748903e-02,
        .3215750886494999,
        .4594254726138241}};
    std::array<double, Order + 1> Numerators{{
        .1145125051763383,
        .4580500207053534,
        .68707503105803,
        .4580500207053534,
        .1145125051763383}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class LowPass<1, 2, 125> {
protected:
    static constexpr std::size_t Order = 2;
    std::array<double, Order> Denominators{{
        0.8951411342166644,
        -1.891269258889966}};
    std::array<double, Order + 1> Numerators{{
        9.679688316745671e-04,
        1.935937663349134e-03,
        9.679688316745671e-04}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class LowPass<5, 3, 125>{
protected:
    static constexpr std::size_t Order = 3;
    std::array<double, Order> Denominators{{
        -.6041096995072748,
        2.115254127003159,
        -2.498608344691178}};
    std::array<double, Order + 1> Numerators{{
        1.567010350588269e-03,
        4.701031051764807e-03,
        4.701031051764807e-03,
        1.567010350588269e-03}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class LowPass<15, 4, 125>{
protected:
    static constexpr std::size_t Order = 4;
    std::array<double, Order> Denominators{{
        .1316807150386918,
        -.7824401031209367,
        1.841785841678163,
        -2.048395137764509}};
    std::array<double, Order + 1> Numerators{{
        8.91445723946302e-03,
        3.565782895785208e-02,
        5.348674343677812e-02,
        3.565782895785208e-02,
        8.91445723946302e-03}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

template<>
class LowPass<30, 2, 250>{
protected:
    static constexpr std::size_t Order = 2;
    std::array<double, Order> Denominators{{
        0.58691950806119031,
        -1.4754804435926461}};
    std::array<double, Order + 1> Numerators{{
        0.027859766117136031,
        0.055719532234272062,
        0.027859766117136031}};
    std::array<double, Order> V{make_array<double, Order>(0.0)};
};

}

#endif // LOW_PASS_FILTER_H
