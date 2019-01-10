extern "C"
{
#include "cchannels.h"
}

#include "cchannel-helper.h"
#include "filter/iir_filter.h"
#include "filter/cascade_filter.h"
#include "filter/band_stop_filter.h"
#include "filter/high_pass_filter.h"
#include "filter/low_pass_filter.h"
#include "event_listener.h"
#include "channels/data_channel.h"
#include "sdk_error.h"

std::unique_ptr<DSP::DigitalFilter<double>> createFilter(Filter filter) {
	if (filter == LowPass_1Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<1, 2, 125>>>();
	}
	if (filter == LowPass_1Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<1, 2, 125>>>();
	}
	if (filter == LowPass_5Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<5, 3, 125>>>();
	}
	if (filter == LowPass_5Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<5, 3, 125>>>();
	}
	if (filter == LowPass_15Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<15, 4, 125>>>();
	}
	if (filter == LowPass_15Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<15, 4, 125>>>();
	}
	if (filter == LowPass_27Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<27, 4, 125>>>();
	}
	if (filter == LowPass_27Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<27, 4, 125>>>();
	}
	if (filter == LowPass_30Hz_SF250) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::LowPass<30, 2, 250>>>();
	}
	if (filter == LowPass_30Hz_SF250_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::LowPass<30, 2, 250>>>();
	}
	if (filter == HighPass_2Hz_SF250) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<2, 2, 250>>>();
	}
	if (filter == HighPass_2Hz_SF250_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<2, 2, 250>>>();
	}
	if (filter == HighPass_3Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<3, 2, 125>>>();
	}
	if (filter == HighPass_3Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<3, 2, 125>>>();
	}
	if (filter == HighPass_5Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<5, 3, 125>>>();
	}
	if (filter == HighPass_5Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<5, 3, 125>>>();
	}
	if (filter == HighPass_11Hz_SF125) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::HighPass<11, 3, 125>>>();
	}
	if (filter == HighPass_11Hz_SF125_Reverse) {
		return std::make_unique<DSP::IIRReverseFilter<DSP::HighPass<11, 3, 125>>>();
	}
	if (filter == BandStop_45_55Hz_SF250) {
		return std::make_unique<DSP::IIRForwardFilter<DSP::BandStop<45, 55, 4, 250>>>();
	}
	throw std::runtime_error("Filter is not supported");
}

std::unique_ptr<DSP::DigitalFilter<double>>  getCompoundFilter(Filter *filters, size_t filter_count) {
	std::vector<std::unique_ptr<DSP::DigitalFilter<double>>> digitalFilters;
	digitalFilters.reserve(filter_count);
	for (size_t i = 0; i != filter_count; ++i) {
		try {
			digitalFilters.push_back(createFilter(filters[i]));
		}
		catch (...) {
			continue;
		}
	}
	auto cascadeFilter = DSP::make_cascade_filter<std::unique_ptr>(std::move(digitalFilters));
	return std::make_unique<decltype(cascadeFilter)>(std::move(cascadeFilter));
}

