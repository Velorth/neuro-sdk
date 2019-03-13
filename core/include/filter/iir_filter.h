#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "digital_filter.h"
#include "direction_strategy.h"
#include "filter_algorithm.h"

namespace DSP {

template <typename T, typename Passband, template <class> class Algorithm, template <class> class DirectionStrategy = ForwardFiltering>
class IIRFilter : public DigitalFilter<T>, private DirectionStrategy<Algorithm<Passband>> {
public:
    T filter(T sample) override{
        return this->performFiltering(sample);
    }
};

template <class Passband, typename T = double>
using IIRForwardFilter = IIRFilter<T, Passband, FilterSolutionAlgorithm>;

template <class Passband, typename T = double>
using IIRReverseFilter = IIRFilter<T, Passband, FilterSolutionAlgorithm, ReverseFiltering>;

}

#endif // IIR_FILTER_H
