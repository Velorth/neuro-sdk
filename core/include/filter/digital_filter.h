#ifndef DIGITAL_FILTER_H
#define DIGITAL_FILTER_H

namespace DSP {

template <typename T>
class DigitalFilter{
public:
	using SampleType = T;

	virtual ~DigitalFilter() = default;
    virtual T filter(T) = 0;
};

}

#endif //DIGITAL_FILTER_H
