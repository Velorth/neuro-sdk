#ifndef EEG_CHANNELS_H
#define EEG_CHANNELS_H

#include "lib_export.h"
#include "cspectrum-channel.h"

typedef struct _SpectrumPowerDoubleChannel SpectrumPowerDoubleChannel;

SDK_SHARED SpectrumPowerDoubleChannel* create_SpectrumPowerDoubleChannel(
	SpectrumDoubleChannel **channels, 
	size_t channels_count, 
	float low_freq, float high_freq,
	char *name,
	double window_duration, double overlapping_coeff);

#endif