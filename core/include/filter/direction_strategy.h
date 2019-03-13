#ifndef DIRECTION_STRATEGY_H
#define DIRECTION_STRATEGY_H

namespace DSP {

template <typename FilterAlgorithm>
class ForwardFiltering : private FilterAlgorithm {
protected:
    template <typename T>
    T performFiltering(T sample){
        return this->filter(sample);
    }
};

template <typename FilterAlgorithm>
class ReverseFiltering : private FilterAlgorithm {
protected:
    template <typename T>
    T performFiltering(T sample){
        return sample - this->filter(sample);
    }
};

}

#endif // DIRECTION_STRATEGY_H
