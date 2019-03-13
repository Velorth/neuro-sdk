#ifndef FILTER_ALGORITHM_H
#define FILTER_ALGORITHM_H

namespace DSP {

template <typename PassbandType>
class ClassicFilterAlgorithm : private PassbandType{
protected:
    template <typename T>
    auto filter(const T &sample){
        auto V0 = sample;
        for (auto i = PassbandType::Order; i > 0; i--) {
            V0 = V0 - this->Denominators[i] * this->V[i - 1];
        }
        auto Y = V0 * this->Numerators[0] + this->V[0] * this->Numerators[1];
        for (auto i = PassbandType::Order; i > 1; i--) {
            Y = Y + this->Numerators[i] * this->V[i - 1];
            this->V[i - 1] = this->V[i - 2];
        }
        this->V[0] = V0;
        return Y;
    }
};

template <typename PassbandType>
class FilterSolutionAlgorithm : private PassbandType{
protected:
    template <typename T>
    T filter(T invar){
        T sumden = 0.0, sumnum = 0.0;
        for (auto i = decltype(PassbandType::Order){0}; i < PassbandType::Order; i++) {
            sumden += this->V[i] * this->Denominators[i];
            sumnum += this->V[i] * this->Numerators[i];
            if (i < PassbandType::Order - 1) this->V[i] = this->V[i + 1];
        }
        this->V[PassbandType::Order - 1] = invar - sumden;
        sumnum += this->V[PassbandType::Order - 1] * this->Numerators[PassbandType::Order];
        return sumnum;
    }
};

}

#endif // FILTER_ALGORITHM_H
