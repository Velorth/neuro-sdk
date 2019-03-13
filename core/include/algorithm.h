/*
 * Copyright 2016 - 2017 Neurotech MRC. http://neurotech.ru/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <limits>
#include <complex>
#include <array>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

namespace alg {

    constexpr double Pi = 3.14159265358979323846;
    constexpr double TwoPi = 6.283185307179586;

    template <typename T>
    T dispersion(const std::vector<T> &vector) {
        T expectedValue = 0.0;
        for (auto& sample : vector){
            expectedValue+=sample;
        }
        expectedValue/=vector.size();

        T dispersion = 0.0;
        for (auto& sample : vector){
            dispersion += std::pow(sample - expectedValue, 2);
        }

        dispersion/=vector.size()-1;

        return dispersion;
    }

    template<typename T>
    constexpr T log_base_2(T n) {
        return ((n < 2) ? 1 : 1 + log_base_2(n / 2));
    }

    template<
            typename T,
            typename = typename std::enable_if<std::is_integral<T>::value>::type,
            typename = typename std::enable_if<std::is_unsigned<T>::value>::type
    >
    constexpr T next_power_2(T value, unsigned max_bit = std::numeric_limits<T>::digits,
                             unsigned current_bit = 1) {

        return max_bit <= current_bit ? value : next_power_2(
                ((value - 1) | ((value - 1) >> current_bit)) + 1, max_bit, current_bit << 1);
    }

    template <typename T>
    std::complex<T> w(std::size_t k, std::size_t N){

        if (k % N == 0) return 1;
        T arg = -2 * Pi * k / N;
        return std::complex<T>(std::cos(arg), std::sin(arg));
    }

    template <typename T>
    std::complex<T> w1(std::size_t k, std::size_t N){

        if (k % N == 0) return 1;
        T arg = 2 * Pi * k / N;
        return std::complex<T>(std::cos(arg), std::sin(arg));
    }

    template <typename T>
    std::vector<std::complex<T>> complex_dft(const std::vector<std::complex<T>> &x) {
        auto N = x.size();
        std::vector<std::complex<T>> x1(N);
        std::vector<std::complex<T>> Out(N);

        std::complex<T> i(0, 1);

        for (decltype(N) k = 0; k < N; k++) {
            for (decltype(N) n = 0; n < N; n++) {
                x1[n] = x[n] * std::exp((-2 * Pi * k * n * i) / static_cast<T>(N));
            }

            Out[k] = std::accumulate(x1.begin(), x1.end(), std::complex<T>(0,0), [](const std::complex<T> &cx, const std::complex<T> &cy){
                return cx + cy;
            });
        }

        return Out;
    }

    template <typename T>
    std::vector<std::complex<T>> complex_idft(const std::vector<std::complex<T>> &x) {
        auto N = x.size();
        std::vector<std::complex<T>> x1(N);
        std::vector<std::complex<T>> Out(N);

        std::complex<T> i(0, 1);

        for (decltype(N) k = 0; k < N; k++) {
            for (decltype(N) n = 0; n < N; n++) {
                x1[n] = x[n] * std::exp((2 * Pi * k * n  * i) / static_cast<T>(N));
            }

            Out[k] = std::accumulate(x1.begin(), x1.end(), std::complex<T>(0,0), [](const std::complex<T> &cx, const std::complex<T> &cy){
                return cx + cy;
            })/static_cast<T>(N);
        }

        return Out;
    }

    template <typename T>
    std::vector<std::complex<T>> complex_fft(const std::vector<std::complex<T>> &data) {

        auto N = data.size();
        std::vector<std::complex<T>> X(N);
        if (N == 2) {
            X[0] = data[0] + data[1];
            X[1] = data[0] - data[1];
        }
        else {
            std::vector<std::complex<T>> data_even(N / 2);
            std::vector<std::complex<T>> data_odd(N / 2);
            for (decltype(N) i = 0; i < N / 2; i++){

                data_even[i] = data[2 * i];
                data_odd[i] = data[2 * i + 1];
            }
            std::vector<std::complex<T>> X_even = complex_fft(data_even);
            std::vector<std::complex<T>> X_odd = complex_fft(data_odd);
            for (decltype(N) i = 0; i < N / 2; i++)
            {
                X[i] = X_even[i] + w<T>(i, N) * X_odd[i];
                X[i + N / 2] = X_even[i] - w<T>(i, N) * X_odd[i];
            }
        }
        return X;
    }

    template <typename T>
    std::vector<std::complex<T>> complex_ifft(const std::vector<std::complex<T>> &data) {

        auto N = data.size();
        std::vector<std::complex<T>> X(N);
        if (N == 2) {

            X[0] = data[0] + data[1];
            X[1] = data[0] - data[1];
        }
        else {

            std::vector<std::complex<T>> data_even(N / 2);
            std::vector<std::complex<T>> data_odd(N / 2);
            for (decltype(N) i = 0; i < N / 2; i++){

                data_even[i] = data[2 * i];
                data_odd[i] = data[2 * i + 1];
            }
            std::vector<std::complex<T>> X_even = complex_ifft(data_even);
            std::vector<std::complex<T>> X_odd = complex_ifft(data_odd);
            for (decltype(N) i = 0; i < N / 2; i++)
            {
                X[i] = X_even[i] + w1<T>(i, N) * X_odd[i];
                X[i + N / 2] = X_even[i] - w1<T>(i, N) * X_odd[i];
            }
        }

        return X;
    }

    template <typename T>
    std::vector<T> fft(const std::vector<T> &data) {

        std::vector<std::complex<T>> complexData(data.size());
        std::copy(data.begin(), data.end(), complexData.begin());
        auto complexResult = complex_fft(complexData);
        std::vector<T> result(complexResult.size());
        std::transform(complexResult.begin(), complexResult.end(), result.begin(),
                       [](const std::complex<T> &c) { return std::abs(c); });
        return result;
    }

    template <typename T>
    std::vector<T> ifft(const std::vector<T> &data) {

        std::vector<std::complex<T>> complexData(data.size());
        std::copy(data.begin(), data.end(), complexData.begin());
        auto complexResult = complex_ifft(complexData);
        std::vector<T> result(complexResult.size());
        std::transform(complexResult.begin(), complexResult.end(), result.begin(),
                       [](const std::complex<T> &c) { return std::norm(c); });
        return result;
    }

    template<typename T, std::size_t size>
    std::array<T, next_power_2(size)> fft(const std::array<T, next_power_2(size)> &) {

    }

    template <typename T>
    T spectrum_power(T *spectrum, float frequency_step, float start_freq,
                               float stop_freq, float normalization_coefficient) {
        auto startIndex = (int) (start_freq / frequency_step);
        auto stopIndex = (int) (stop_freq / frequency_step);

        auto spectrumPower = 0.0;
        for (auto i = startIndex; i <= stopIndex; ++i) {
            spectrumPower += spectrum[i];
        }

        return spectrumPower * normalization_coefficient;
    }

    template <typename T>
    std::vector<T> cross_correlation(std::vector<T> inp_a, std::vector<T> inp_b) {

        if (inp_a.size()==0 || inp_b.size()==0)
            return std::vector<T>();

        auto n = (inp_a.size() > inp_b.size()) ? next_power_2(inp_a.size()) : next_power_2(inp_b.size());
        inp_a.resize(n);
        inp_b.resize(n);

        std::vector<std::complex<T>> a(inp_a.begin(), inp_a.end());
        std::vector<std::complex<T>> b(inp_b.begin(), inp_b.end());

        auto swa = complex_fft(a);
        auto swb = complex_fft(b);

        decltype(swa) multiplied(swa.size());
        std::transform(swa.begin(), swa.end(), swb.begin(), multiplied.begin(), std::multiplies<std::complex<T>>());

        auto crossCorrelationComplex = complex_ifft(multiplied);
        auto N = crossCorrelationComplex.size();
        std::transform(crossCorrelationComplex.begin(), crossCorrelationComplex.end(), crossCorrelationComplex.begin(),[N](const std::complex<T> &v){
            return v/static_cast<T>(N);
        });
        std::vector<T> crossCorrelation(crossCorrelationComplex.size());
        auto dispersionA = dispersion(inp_a);
        auto dispersionB = dispersion(inp_b);
        auto dispersionCoefficient = 1.f / (std::sqrt(dispersionA * dispersionB) * n);
        std::transform(crossCorrelationComplex.begin(),
                       crossCorrelationComplex.end(),
                       crossCorrelation.begin(),
                       [dispersionCoefficient](const std::complex<T> &complex) {
                           return complex.real() * dispersionCoefficient;
                       });

        return crossCorrelation;
    }
}
#endif //ALGORITHM_H
