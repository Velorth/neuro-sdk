#ifndef CSPECTRUM_CHANNEL_H
#define CSPECTRUM_CHANNEL_H

#include "cchannels.h"

typedef struct _SpectrumDoubleChannel SpectrumDoubleChannel;

SDK_SHARED SpectrumDoubleChannel* create_SpectrumDoubleChannel(DoubleChannel *);
SDK_SHARED int SpectrumDoubleChannel_get_hz_per_spectrum_sample(SpectrumDoubleChannel* channel, double* out_step);
SDK_SHARED int SpectrumDoubleChannel_get_spectrum_length(SpectrumDoubleChannel *channel, size_t *out_spectrum_length);

#endif // CSPECTRUM_CHANNEL_H
