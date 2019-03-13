#ifndef CASCADE_FILTER_H
#define CASCADE_FILTER_H

#include "digital_filter.h"
#include <utility>

namespace DSP {

template <
	typename T, 
	template <typename...> class FilterContainer,
	template <typename...> class SmartPtr,
	template <typename> class FilterBaseType = DigitalFilter
>
class CascadeFilter : public DigitalFilter<T> {
public:
	using ContainerType = FilterContainer<SmartPtr<FilterBaseType<T>>>;

	template <typename FilterPointerInputIterator>
	CascadeFilter(FilterPointerInputIterator begin, FilterPointerInputIterator end) :
		mFilterCollection(begin, end)
	{}

	explicit CascadeFilter(const ContainerType &filter_collection) :
		mFilterCollection(filter_collection)
	{}

	explicit CascadeFilter(ContainerType &&filter_collection) :
		mFilterCollection(std::move(filter_collection))
	{}

	T filter(T sample) override {
		for (const auto& filter : mFilterCollection) {
			sample = filter->filter(sample);
		}
		return sample;
	}
private:
	ContainerType mFilterCollection;
};

template <
	template <typename...> class SmartPtr,
	template <typename...> class Container,
	template <typename> class FilterBaseType = DigitalFilter,
	typename... ContainerArgs
>
auto make_cascade_filter(Container<ContainerArgs...> &&collection) {
	using T = typename Container<ContainerArgs...>::value_type::element_type::SampleType;
	return CascadeFilter<T, Container, SmartPtr, FilterBaseType>(std::move(collection));
}

template <
	template <typename...> class SmartPtr,
	template <typename, typename...> class Container,
	template <typename> class FilterBaseType = DigitalFilter,
	typename... ContainerArgs
>
auto make_cascade_filter(const Container<ContainerArgs...> &collection) {
	using T = typename Container<ContainerArgs...>::value_type::element_type::SampleType;
	return CascadeFilter<T, Container, SmartPtr, FilterBaseType>(collection);
}

}
#endif // CASCADE_FILTER_H
